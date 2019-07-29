/**********************************************************************/

#ifndef JSONHANDLER_JSON_UTIL
#define JSONHANDLER_JSON_UTIL

#include "./json_value.hpp"
#include <iosfwd>
#include <string>
#include <stdint.h>
#include <map>

#define JSONHANDLER_JSON_MAX_DEPTH	500
#define JSONHANDLER_JSON_MAX_LENGTH  0

namespace jsonhandler {
    
class JsonValue;
struct JsonParseError;

/** Encodes a JsonValue object as a JSON UTF8 string. The provided json value must be valid as a
    json expression (meaning that it must be a map or a list)
-----
@param jsonValue        the JsonValue to encode
@param result           the string to output the JSON to
@param makePretty       true to format the result output, default is false
*/
void JsonEncode(const JsonValue&    jsonValue,
                std::string&        result,
                bool makePretty = false);

std::string JsonEncode(const std::string& value);
std::string JsonEncode(double value);
std::string JsonEncode(bool value);
std::string JsonEncodeNull();

bool JsonDecodeToFlexBuffer(std::istream&       stream,
                flexbuffers::Builder& flexbuilder,
                unsigned int        maxDepth = JSONHANDLER_JSON_MAX_DEPTH,
                unsigned int        maxLength = JSONHANDLER_JSON_MAX_LENGTH);
    
bool JsonDecode(JsonValue&          jsonValue,
                std::istream&       stream,
                JsonParseError&     parseError,
                unsigned int        maxDepth = JSONHANDLER_JSON_MAX_DEPTH,
                unsigned int        maxLength = JSONHANDLER_JSON_MAX_LENGTH);

bool JsonDecodeToFlexBuffer(const std::string&  input,
                flexbuffers::Builder& flexbuilder,
                unsigned int        maxDepth = JSONHANDLER_JSON_MAX_DEPTH,
                unsigned int        maxLength = JSONHANDLER_JSON_MAX_LENGTH);
    
bool JsonDecode(JsonValue&          jsonValue,
                const std::string&  input,
                JsonParseError&     parseError,
                unsigned int        maxDepth = JSONHANDLER_JSON_MAX_DEPTH,
                unsigned int        maxLength = JSONHANDLER_JSON_MAX_LENGTH);

struct JsonParseError
{
    std::streampos  position;
    std::string     what;
};
    
/** For a jsonValue of type number, returns it as the nearest integer.
    Rounding is to the nearest integer.
    If the value is not a number MismatchedType is thrown.
*/
int64_t GetIntegerValue(const JsonValue& jsonValue);


template <typename T>
struct JsonTypeTrait
{
};

template <>
struct JsonTypeTrait<bool>
{
    static bool ValidateType(JsonValue::Type type)
    {
        return JsonValue::type_Bool == type;
    }
    
    static void Copy(const JsonValue& jsonValue, bool& value)
    {
        value = jsonValue.GetBool();
    }
};

template <>
struct JsonTypeTrait<std::string>
{
    static bool ValidateType(JsonValue::Type type)
    {
        return JsonValue::type_String == type;
    }
    
    static void Copy(const JsonValue& jsonValue, std::string& value)
    {
        value = jsonValue.GetString();
    }
};

template <>
struct JsonTypeTrait<int>
{
    static bool ValidateType(JsonValue::Type type)
    {
        return JsonValue::type_Number == type;
    }
    
    static void Copy(const JsonValue& jsonValue, int& value)
    {
        value = static_cast<int>(jsonValue.GetInt32());
    }
};

template <>
struct JsonTypeTrait<long>
{
    static bool ValidateType(JsonValue::Type type)
    {
        return JsonValue::type_Number == type;
    }
    
    static void Copy(const JsonValue& jsonValue, long& value)
    {
        value = static_cast<long>(jsonValue.GetNumber());
    }
};

template <>
struct JsonTypeTrait<double>
{
    static bool ValidateType(JsonValue::Type type)
    {
        return JsonValue::type_Number == type;
    }
    
    static void Copy(const JsonValue& jsonValue, double& value)
    {
        value = jsonValue.GetNumber();
    }
};

template <>
struct JsonTypeTrait<float>
{
    static bool ValidateType(JsonValue::Type type)
    {
        return JsonValue::type_Number == type;
    }
    
    static void Copy(const JsonValue& jsonValue, float& value)
    {
        value = static_cast<float>(jsonValue.GetNumber());
    }
};

template <>
struct JsonTypeTrait<JsonValueMap>
{
    static bool ValidateType(JsonValue::Type type)
    {
        return JsonValue::type_Map == type;
    }
    
    static void Copy(const JsonValue& jsonValue, JsonValueMap& value)
    {
        value = jsonValue.GetMap();
    }
};

template <>
struct JsonTypeTrait<JsonValueList>
{
    static bool ValidateType(JsonValue::Type type)
    {
        return JsonValue::type_List == type;
    }
    
    static void Copy(const JsonValue& jsonValue, JsonValueList& value)
    {
        value = jsonValue.GetList();
    }
};
/** Return true if the provided object contains the provided property
*/
inline bool HasProperty(const JsonValueMap& jsonMap, const std::string& key)
{
    const JsonValueMap::const_iterator location = jsonMap.find(key);
    return (location != jsonMap.end());
}

inline bool HasProperty(const JsonValue& jsonValue, const std::string& key)
{
	if (jsonValue.GetType() != JsonValue::type_Map)
		return false;
	return HasProperty(jsonValue.GetMap(), key);
}

/** Return the value of an optional key. If the key cannot be found, or if the
key of not of the right type, false is returned
*/
template <typename T>
bool GetOptionalValue(const JsonValueMap& jsonMap, const std::string& key, T& value)
{
    const JsonValueMap::const_iterator location = jsonMap.find(key);
    const bool hasValidKey = (location != jsonMap.end() && JsonTypeTrait<T>::ValidateType((*location).second.GetType()));
    if (hasValidKey)
    {
        JsonTypeTrait<T>::Copy((*location).second, value);
    }
    return hasValidKey;
}
const JsonValueList* GetOptionalList(const JsonValueMap& jsonMap, const char* key);
inline const JsonValueList* GetOptionalList(const JsonValue& jsonValue, const char* key)
{
    return GetOptionalList(jsonValue.GetMap(), key);
}

const JsonValueMap* GetOptionalMap(const JsonValueMap& jsonMap, const char* key);
inline const JsonValueMap* GetOptionalMap(const JsonValue& jsonValue, const char* key)
{
    return GetOptionalMap(jsonValue.GetMap(), key);
}
}

#endif // JSONHANDLER_JSON_UTIL
