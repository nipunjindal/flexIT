/**********************************************************************/

// self
#include "json_value.hpp"

#include <string>
#include <stdexcept>

#if defined (__GNUC__) && defined (__LINUX__)
#include <cstring>
#endif

namespace jsonhandler {

    
Exception::
Exception(std::string message) throw() :
std::exception(),
mMessage(message)
{
}

Exception::
~Exception() throw()
{
}

const char* Exception::what() const throw()
{
    return mMessage.c_str();
}
    
const JsonValue&
JsonValue::True()
{
    static const JsonValue val_true(true);
    return val_true;
}

const JsonValue&
JsonValue::False()
{
    static const JsonValue val_false(false);    
    return val_false;
}

const JsonValue&
JsonValue::Null()
{
    static const JsonValue val_null;
    return val_null;
}

JsonValue::
JsonValue(Type type) :
	JsonValue()
{
	mType = type;
    switch (mType)
    {
        case type_Map:
        {
            mValue.mMap = new JsonValueMap();
        }
        break;
        case type_List:
        {
            mValue.mList = new JsonValueList();
        }
        break;
        case type_String:
        {
            mValue.mString = new std::string;
        }
        break;
        case type_Number:
        {
            mValue.mDouble = 0.0;
            mNumericType = NumericType::Double;
        }
        break;
        case type_Bool:
        {
            mValue.mBoolean = false;
        }
        break;
        
        default:    // intentional fall through
        case type_Null:
        break;
    }
    mType = type;
}

JsonValue::
JsonValue(const JsonValueMap& map) :
    JsonValue()
{
    SetMap(map);
}

JsonValue::
JsonValue(JsonValueMap&& map) :
    JsonValue()
{
    SetMap(std::move(map));
}

JsonValue::
JsonValue(const JsonValueList& list) :
    JsonValue()
{
    SetList(list);
}

JsonValue::
JsonValue(JsonValueList&& list) :
    JsonValue()
{
    SetList(std::move(list));
}

JsonValue::
JsonValue(const std::string& str) :
    JsonValue()
{
    SetString(str);
}

JsonValue::
JsonValue(std::string&& str) :
    JsonValue()
{
    SetString(std::move(str));
}

JsonValue::
JsonValue(const char* str) :
    JsonValue()
{
	if (str == nullptr)
		str = "";

    SetString(str);
}

JsonValue::
~JsonValue()
{
    Clear();
}

JsonValue::
JsonValue(const JsonValue& rhs) :
    JsonValue()
{
    Copy(rhs);
}

JsonValue::
JsonValue(JsonValue&& rhs) :
    JsonValue()
{
    Swap(rhs);
}

JsonValue&
JsonValue::
operator=(const JsonValue& rhs)
{
    if (this != &rhs)
    {
        Copy(rhs);
    }
    return *this;
}

JsonValue&
JsonValue::
operator=(JsonValue&& rhs)
{
    if (this != &rhs)
    {
        Swap(rhs);
    }
    return *this;
}

JsonValue&
JsonValue::
operator[](const std::string& key)
{
    JsonValueMap& object = GetMap();
    return object[key];
}

JsonValue&
JsonValue::
operator[](int index)
{
    JsonValueList& array = GetList();
    assert(index >= 0 && static_cast<JsonValueList::size_type>(index) < array.size());
    
    return array[static_cast<JsonValueList::size_type>(index)];
}

const JsonValue&
JsonValue::
operator[](int index) const
{
    const JsonValueList& array = GetList();
    assert(index >= 0 && static_cast<JsonValueList::size_type>(index) < array.size());
    
    return array[static_cast<JsonValueList::size_type>(index)];
}

bool
JsonValue::
operator==(const JsonValue& rhs) const
{
    // Quick return if the types do not match
    if (mType != rhs.mType)
        return false;
    
    bool result = false;

    switch (mType)
    {
        case type_Map:
            result = *mValue.mMap == *rhs.mValue.mMap;
            break;
        case type_List:
            result = *mValue.mList == *rhs.mValue.mList;
            break;
        case type_String:
            result = *mValue.mString == *rhs.mValue.mString;
            break;
        case type_Number:
        	if (mNumericType == rhs.mNumericType)
        	{
				switch (mNumericType)
				{
					case NumericType::Double:	result = (mValue.mDouble == rhs.mValue.mDouble);	break;
					case NumericType::Int32: 	result = (mValue.mInt32 == rhs.mValue.mInt32);		break;
					case NumericType::UInt32: 	result = (mValue.mUInt32 == rhs.mValue.mUInt32);	break;
					case NumericType::Int64: 	result = (mValue.mInt64 == rhs.mValue.mInt64);		break;
					case NumericType::Invalid:	result = false; break;
				}
    		}
            break;
        case type_Bool:
            result = mValue.mBoolean == rhs.mValue.mBoolean;
            break;
        case type_Null:
            // JsonValue of type NULL holds no value, so it's enough proof that
            // the types are equivalent.
            result = true;
            break;
    }
    
    return result;
}

bool
JsonValue::
operator!=(const JsonValue& other) const
{
    return !(*this == other);
}

void
JsonValue::
Copy(const JsonValue& rhs)
{
    switch (rhs.GetType())
    {
        case type_Map:
            SetMap(rhs.GetMap());
            break;
        case type_List:
            SetList(rhs.GetList());
            break;
        case type_String:
            SetString(rhs.GetString());
            break;
        case type_Number:
        	{
        		Clear();
        		mType = rhs.GetType();
        		mNumericType = rhs.GetNumericType();
        		switch (mNumericType)
        		{
					case NumericType::Double:	mValue.mDouble = rhs.mValue.mDouble;	break;
					case NumericType::Int32: 	mValue.mInt32 = rhs.mValue.mInt32;		break;
					case NumericType::UInt32: 	mValue.mUInt32 = rhs.mValue.mUInt32;	break;
					case NumericType::Int64: 	mValue.mInt64 = rhs.mValue.mInt64;		break;
					case NumericType::Invalid:
						{
							assert(false);
						}
						break;
        		}
			}
            break;
        case type_Bool:
            SetBool(rhs.GetBool());
            break;
        case type_Null:
            SetNull();
            break;
    }  
}

void
JsonValue::
Clear()
{
    // Deletes any allocated memory (remember C++ allows delete on a NULL object).
    // Although not necessary, sets the internal data to a sentinel value (0).

    switch (mType)
    {
        case type_Map:
            delete mValue.mMap;
            mValue.mMap = nullptr;
            break;
        case type_List:
            delete mValue.mList;
            mValue.mList = nullptr;
            break;
        case type_String:
            delete mValue.mString;
            mValue.mString = nullptr;
            break;
        case type_Number:
        	mNumericType = NumericType::Double;
            mValue.mDouble = 0.0;
            break;
        case type_Bool:
            mValue.mBoolean = false;
            break;
        case type_Null:
            break;
    }

    mType = type_Null;
}

const JsonValueMap&
JsonValue::
GetMap() const
{
    assert(type_Map == mType && nullptr != mValue.mMap);
    return *mValue.mMap;
}

JsonValueMap&
JsonValue::
GetMap()
{
    assert(type_Map == mType && nullptr != mValue.mMap);
    return *mValue.mMap;
}

const JsonValueList&
JsonValue::
GetList() const
{
    assert(type_List == mType && nullptr != mValue.mList);
    return *mValue.mList;
}

JsonValueList&
JsonValue::
GetList()
{
    assert(type_List == mType && nullptr != mValue.mList);
    return *mValue.mList;
}

const std::string&
JsonValue::
GetString() const
{
    assert(type_String == mType && nullptr != mValue.mString);
    return *mValue.mString;
}

std::string&
JsonValue::
GetString()
{
    assert(type_String == mType && nullptr != mValue.mString);
    return *mValue.mString;
}

double
JsonValue::
GetNumber() const
{
    assert(type_Number == mType);
    return GetNumericValue<double>();
}

int32_t
JsonValue::GetInt32() const
{
    assert(type_Number == mType);
    return GetNumericValue<int32_t>();
}

uint32_t
JsonValue::GetUInt32() const
{
    assert(type_Number == mType);
    return GetNumericValue<uint32_t>();
}

int64_t
JsonValue::GetInt64() const
{
    assert(type_Number == mType);
    return GetNumericValue<int64_t>();
}

bool
JsonValue::
GetBool() const
{
    assert(type_Bool == mType);
    return mValue.mBoolean;
}

void
JsonValue::
SetMap(const JsonValueMap& inMap)
{
    Clear();
    mValue.mMap = new JsonValueMap(inMap);
    mType = type_Map;
}

void
JsonValue::
SetMap(JsonValueMap&& inMap)
{
    Clear();
    mValue.mMap = new JsonValueMap(std::move(inMap));
    mType = type_Map;
}

void
JsonValue::
SetList(const JsonValueList& inList)
{
    Clear();
    mValue.mList = new JsonValueList(inList);
    mType = type_List;
}

void
JsonValue::
SetList(JsonValueList&& inList)
{
    Clear();
    mValue.mList = new JsonValueList(std::move(inList));
    mType = type_List;
}

void
JsonValue::
SetString(const std::string& inStr)
{
    Clear();
    mValue.mString = new std::string(inStr);
    mType = type_String;
}

void
JsonValue::
SetString(std::string&& inStr)
{
    Clear();
    mValue.mString = new std::string(std::move(inStr));
    mType = type_String;
}

void
JsonValue::
SetNumber(double inDouble)
{
    Clear();
    mValue.mDouble = inDouble;
    mType = type_Number;
    mNumericType = NumericType::Double;
}

void
JsonValue::
SetNumber(int32_t inValue)
{
    Clear();
    mValue.mInt32 = inValue;
    mType = type_Number;
    mNumericType = NumericType::Int32;
}

void
JsonValue::
SetNumber(uint32_t inValue)
{
    Clear();
    mValue.mUInt32 = inValue;
    mType = type_Number;
    mNumericType = NumericType::UInt32;
}

void
JsonValue::
SetNumber(int64_t inValue)
{
    Clear();
    mValue.mInt64 = inValue;
    mType = type_Number;
    mNumericType = NumericType::Int64;
}

void
JsonValue::
SetBool(bool inBool)
{
    Clear();
    mValue.mBoolean = inBool;
    mType = type_Bool;
}

void
JsonValue::
SetNull()
{
    Clear();
    mType = type_Null;
}

void
JsonValue::
Swap(jsonhandler::JsonValue &other)
{
    std::swap(mType, other.mType);
    std::swap(mNumericType, other.mNumericType);
    std::swap(mValue, other.mValue);
}

JsonValueMap::iterator
Find(   JsonValue& jsonValue,
        const std::string& key)
{
    JsonValueMap& object = jsonValue.GetMap();
    return object.find(key);
}

JsonValueMap::const_iterator
Find(   const JsonValue& jsonValue,
        const std::string& key)
{
    const JsonValueMap& object = jsonValue.GetMap();
    return object.find(key);
}

JsonValueMap::iterator
Find(   JsonValueMap& jsonValueMap,
        const std::string& key)
{
    return jsonValueMap.find(key);
}

JsonValueMap::const_iterator
Find(   const JsonValueMap& jsonValueMap,
        const std::string& key)
{
    return jsonValueMap.find(key);
}

void
swap(JsonValue& v1, JsonValue& v2)
{
    v1.Swap(v2);
}

} // namespace jsonhandler
