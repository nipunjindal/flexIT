/**********************************************************************/

// self
#include "json_util.hpp"
#include "json_value.hpp"
#include "flatbuffers/flexbuffers.h"

// std
#include <stdint.h>
#include <cmath>
#include <cstdlib>
#include <istream>
#include <limits>
#include <sstream>

namespace {

using namespace jsonhandler;

template <typename Encoding>
class IStreamWrapper
{
public:
	typedef typename Encoding::Ch Ch;
    
public:
    IStreamWrapper(std::istream& stream) :
        mStream(&stream)
    {
        // This space left intentionally blank
    }

    Ch Peek() const
    {
        const Ch value = static_cast<Ch>(mStream->peek());
        const bool isGood = mStream->good();
        return (isGood)? value : '\0';
    }

    inline Ch Take()
    {
        return static_cast<Ch>(mStream->get());
    }

    inline std::size_t Tell()
    {
        return static_cast<std::size_t>(mStream->tellg());
    }
    
    // Not needed since our parser is non-destructive (we only read not write)
    inline Ch* PutBegin()              { return 0; }
	inline void Put(Ch c)              { /* This space left intentionally blank */}
	inline size_t PutEnd(Ch* begin)    { return 0; }

private:
    std::istream*   mStream;
};
typedef IStreamWrapper<rapidjson::UTF8<> > istream_utf8;

    class ExceededDepthLimit : public jsonhandler::Exception
{
public:
    ExceededDepthLimit() :
        Exception("The number of nested objects has exceeded the limit.")
    { }
};

class ExceededListLimit : public jsonhandler::Exception
{
public:
    ExceededListLimit() :
        Exception("The number of objects in the array has exceeded the limit.")
    { }
};

/// JsonReaderHandler is the event handler for the rapidjson SAX style parser
class JsonReaderHandler : public
	rapidjson::BaseReaderHandler<rapidjson::UTF8<>, JsonReaderHandler>
{
public:
    explicit JsonReaderHandler( jsonhandler::JsonValue&   outJson,
                                unsigned int                    inMaxDepth,
                                unsigned int                    inMaxLength);
    
    // BasereaderHandler overrides
    bool Null();
    bool Bool(bool b);
    bool Int(int i);
    bool Uint(unsigned i);
    bool Int64(int64_t i);
    bool Uint64(uint64_t i);
    bool Double(double d);
    bool String(const Ch* str, rapidjson::SizeType length, bool copy);
    bool StartObject();
    bool EndObject(rapidjson::SizeType memberCount);
    bool StartArray();
    bool EndArray(rapidjson::SizeType elementCount);

private:
    void InsertJsonValue(jsonhandler::JsonValue& value);

private:
    jsonhandler::JsonValue* mResult;
    
    // As we traverse deeper into the JSON string we're parsing, we need the stack
    // to keep track of parent objects (typed either as a list or a map). The top
    // of the stack tells us where to add new leaf nodes.
    std::vector<jsonhandler::JsonValue*> mStack;
    
    std::string mName;

    unsigned int mMaxDepth;
    unsigned int mMaxLength;
};

} // namespace anonymous

namespace jsonhandler {

bool JsonDecodeToFlexBuffer(std::istream&   input,
                flexbuffers::Builder& flexbuilder,
                unsigned int    maxDepth,
                unsigned int    maxLength)
{
    JsonParseError parseError;
    JsonValue jsonValue;
    rapidjson::Reader reader;
    JsonReaderHandler handler(jsonValue, maxDepth, maxLength);

    rapidjson::ParseResult result;
    try
    {
        istream_utf8 stream(input);
        result = reader.Parse<rapidjson::kParseDefaultFlags>(stream, handler, flexbuilder);
        if (result.IsError ())
        {
            parseError.position = static_cast<std::streampos>(reader.GetErrorOffset());
            parseError.what = rapidjson::GetParseError_En (result.Code());
		}
    }
    catch (const Exception& e)
    {
        parseError.position = input.tellg();
        parseError.what = e.what();
    }

    return result;
}
 
bool JsonDecode(JsonValue&      jsonValue,
                std::istream&   input,
                JsonParseError& parseError,
                unsigned int    maxDepth,
                unsigned int    maxLength)
{
    rapidjson::Reader reader;
    JsonReaderHandler handler(jsonValue, maxDepth, maxLength);
    
    rapidjson::ParseResult result;
    try
    {
        istream_utf8 stream(input);
        result = reader.Parse<rapidjson::kParseDefaultFlags>(stream, handler);
        if (result.IsError ())
        {
            parseError.position = static_cast<std::streampos>(reader.GetErrorOffset());
            parseError.what = rapidjson::GetParseError_En (result.Code());
        }
    }
    catch (const Exception& e)
    {
        parseError.position = input.tellg();
        parseError.what = e.what();
    }
    
    return result;
}
    
template<typename T>
T Round(T inValue)
{
    inValue += (inValue < static_cast<T>(0) ? static_cast<T>(-0.5) : static_cast<T>(0.5));
    // BEWARE: intentional fall thru to "case roundMode_Truncate"!
    return (inValue);
}


bool JsonDecodeToFlexBuffer(const std::string&  input,
                flexbuffers::Builder& flexbuilder,
                unsigned int    	maxDepth,
                unsigned int    	maxLength)
{
	std::stringstream stream(input);
    return ::JsonDecodeToFlexBuffer(stream,
                        flexbuilder,
                		maxDepth,
                		maxLength);
}

bool JsonDecode(JsonValue&          jsonValue,
                const std::string&  input,
                JsonParseError&     parseError,
                unsigned int        maxDepth,
                unsigned int        maxLength)
{
    std::stringstream stream(input);
    return ::JsonDecode(    jsonValue,
                        stream,
                        parseError,
                        maxDepth,
                        maxLength);
}
    
int64_t GetIntegerValue(const JsonValue& jsonValue)
{
    return static_cast<int64_t>(Round(jsonValue.GetNumber()));
}

const JsonValueList* GetOptionalList(const JsonValueMap& jsonMap, const char* key)
{
    const JsonValueList* result = nullptr;
    if (key != nullptr)
    {
        const JsonValueMap::const_iterator iter = jsonMap.find(key);
        if (iter != jsonMap.end())
        {
            const JsonValue& value = (*iter).second;
            result = &(value.GetList());
        }
    }
    return result;
}

const JsonValueMap* GetOptionalMap(const JsonValueMap& jsonMap, const char* key)
{
    const JsonValueMap* result = nullptr;
    if (key != nullptr)
    {
        const JsonValueMap::const_iterator iter = jsonMap.find(key);
        if (iter != jsonMap.end())
        {
            const JsonValue& value = (*iter).second;
            result = &(value.GetMap());
        }
    }
    return result;
}
} // namespace jsonhandler

namespace {

JsonReaderHandler::
JsonReaderHandler(  jsonhandler::JsonValue&   outJson,
                    unsigned int                    inMaxDepth,
                    unsigned int                    inMaxLength) :
    mResult(&outJson),
    mStack(),
    mName(),
    mMaxDepth(inMaxDepth),
    mMaxLength(inMaxLength)
{
    
    mResult->SetNull();
}

void
JsonReaderHandler::
InsertJsonValue(jsonhandler::JsonValue& value)
{
    
    // The stack should never be empty here; it should at least have the return value
    if (mStack.empty())
        return;
    
    // The newly found value needs to be placed within its parent Array or
    // Object container.
    JsonValue* top = mStack.back();
    JsonValue* newMember = NULL;

    if (JsonValue::type_List == top->GetType())
    {
        JsonValueList& array = top->GetList();
        if (mMaxLength > 0 && array.size() > mMaxLength)
        {
            throw ExceededListLimit();
        }

        // Fake move (add a light weight (typeNull) member. Then swap the contents
        array.push_back(JsonValue());
        JsonValue& ref = array.back();
        jsonhandler::swap(ref, value);
        
        newMember = &ref;
    }
    else if (JsonValue::type_Map == top->GetType())
    {
        JsonValueMap& object = top->GetMap();

        std::pair<JsonValueMap::iterator, bool>
            location = object.insert(JsonValueMap::value_type(mName, JsonValue()));
        jsonhandler::JsonValue& newValue = (location.first)->second;
        jsonhandler::swap(newValue, value);
        newMember = &newValue;
        
        // after we use the name we're done with it; make sure to clear it
        mName.clear();
    }

    // Newly found Arrays and Objects need to be placed on the stack until we
    // receive EndArray() or EndObject()
    if (newMember &&
        (JsonValue::type_List == newMember->GetType() || JsonValue::type_Map == newMember->GetType()))
    {
        if (mMaxDepth > 0 && mStack.size() > mMaxDepth)
        {
            throw ExceededDepthLimit();
        }

        mStack.push_back(newMember);
    }
}

bool
JsonReaderHandler::
StartObject()
{
    using namespace jsonhandler;
    if (JsonValue::type_Null == mResult->GetType())
    {
        mResult->SetMap(JsonValueMap());
        mStack.push_back(mResult);
    }
    else
    {
        JsonValue newMap(JsonValue::type_Map);
        InsertJsonValue(newMap);
    }
	return true;
}

bool
JsonReaderHandler::
EndObject(rapidjson::SizeType memberCount)
{
    mStack.pop_back();
	return true;
}

bool
JsonReaderHandler::
StartArray()
{
    if (JsonValue::type_Null == mResult->GetType())
    {
        mResult->SetList(JsonValueList());
        mStack.push_back(mResult);
    }
    else
    {
        JsonValue newList(JsonValue::type_List);
        InsertJsonValue(newList);
    }
	return true;
}

bool
JsonReaderHandler::
EndArray(rapidjson::SizeType elementCount)
{
    mStack.pop_back();
	return true;
}

bool
JsonReaderHandler::
String(const Ch* str, rapidjson::SizeType length, bool copy)
{
    std::string str_value(str, length);

    // we need to determine if this is the name of a member in an object because
    // if it is, we save it until we need it, which is when we receive its
    // corresponding value
    bool insertValue = true;
    if (mName.empty() && !mStack.empty())
    {
        JsonValue* const top = mStack.back();
        if (JsonValue::type_Map == top->GetType())
        {
            mName = str_value;
            insertValue = false;
        }
    }

    if (insertValue)
    {
        JsonValue newString(JsonValue::type_String);
        std::swap(str_value, newString.GetString());
        InsertJsonValue(newString);
    }
	return true;
}
    
bool
JsonReaderHandler::
Null()
{
    JsonValue newValue;
    InsertJsonValue(newValue);
	return true;
}

bool
JsonReaderHandler::
Bool(bool b)
{
    JsonValue newValue(b);
    InsertJsonValue(newValue);
	return true;
}

bool
JsonReaderHandler::
Int(int i)
{
    JsonValue newValue(static_cast<int32_t>(i));
    InsertJsonValue(newValue);
	return true;
}

bool
JsonReaderHandler::
Uint(unsigned i)
{
    
    if (i <= static_cast<uint32_t>(std::numeric_limits<int32_t>::max()))
    {
        JsonValue newValue(static_cast<int32_t>(i));
        InsertJsonValue(newValue);
    }
    else
    {
        JsonValue newValue(static_cast<uint32_t>(i));
        InsertJsonValue(newValue);
    }
	return true;
}

bool
JsonReaderHandler::
Int64(int64_t i)
{
    JsonValue newValue(i);
    InsertJsonValue(newValue);
	return true;
}

bool
JsonReaderHandler::
Uint64(uint64_t i)
{
    JsonValue newValue(static_cast<double>(i));
    InsertJsonValue(newValue);
	return true;
}

bool
JsonReaderHandler::
Double(double d)
{
    JsonValue newValue(d);
    InsertJsonValue(newValue);
	return true;
}

} // namespace anonymous
