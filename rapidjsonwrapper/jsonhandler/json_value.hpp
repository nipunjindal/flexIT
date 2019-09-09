/**********************************************************************/

#ifndef JSONHANDLER_JSON_VALUE_HPP
#define JSONHANDLER_JSON_VALUE_HPP

#include <vector>
#include <map>
#include <memory>
#include <string>
#include "allocators.h"
#include "encodings.h"
#include "reader.h"
#include "writer.h"
#include "error/en.h"
#include "prettywriter.h"

namespace jsonhandler {

class JsonValue;

typedef std::vector<JsonValue> JsonValueList;
typedef std::map<std::string, JsonValue> JsonValueMap;

class JsonValue
{
public:
	// TODO: Switch to enum class
    enum Type: unsigned char
    {
        type_Map,
        type_List,
        type_String,
        type_Number,
        type_Bool,
        type_Null
    };

	enum class NumericType: unsigned char
	{
		Invalid,
		Double,
		Int32,
		UInt32,
		Int64
	};

    static const JsonValue& True();
    static const JsonValue& False();
    static const JsonValue& Null();
    
public:
    /// Creates a type_Null JsonValue object
    constexpr JsonValue() :
    	mType(type_Null),
    	mNumericType(NumericType::Invalid),
    	mValue(false)
	{ }
    
    /// Creates a type_Bool JsonValue object
    constexpr JsonValue(bool boolean) :
    	mType(type_Bool),
    	mNumericType(NumericType::Invalid),
    	mValue(boolean)
	{ }

    // Numeric constructors
    constexpr JsonValue(double value) :
        mType(type_Number),
        mNumericType(NumericType::Double),
        mValue(value)
    { }

    constexpr JsonValue(int32_t value) :
        mType(type_Number),
        mNumericType(NumericType::Int32),
        mValue(value)
    { }

    constexpr JsonValue(uint32_t value) :
        mType(type_Number),
        mNumericType(NumericType::UInt32),
        mValue(value)
    { }

    constexpr JsonValue(int64_t value) :
        mType(type_Number),
        mNumericType(NumericType::Int64),
        mValue(value)
    { }

    /// Creates a default JsonValue of the provided type
	JsonValue(Type type);

    /// Creates a type_Map JsonValue object
    JsonValue(const JsonValueMap& map);
    JsonValue(JsonValueMap&& map);

    /// Creates a type_List JsonValue object
    JsonValue(const JsonValueList& list);
    JsonValue(JsonValueList&& list);

    /// Creates a type_String JsonValue object
    JsonValue(const std::string& str);
    JsonValue(std::string&& str);

    /// Creates a type_String JsonValue object
    JsonValue(const char* str);

    ~JsonValue();
    
    JsonValue(const JsonValue& rhs);
    JsonValue(JsonValue&& rhs);
    
    JsonValue& operator=(const JsonValue& rhs);
    JsonValue& operator=(JsonValue&& rhs);

    /** Returns the existing JsonValue for key or inserts a type_Null 
        JsonValue into the array if key does not exist.
        Throws MismatchedType if the type isn't type_Map.
    */
    JsonValue& operator[](const std::string& key);

    /** throws MismatchedType if the type isn't type_List.
        throws std::out_of_range if index is not in range.
    */
    JsonValue& operator[](int index);

    /** throws MismatchedType if the type isn't type_List.
        throws std::out_of_range if index is not in range.
    */
    const JsonValue& operator[](int index) const;
    
    bool operator==(const JsonValue& other) const;

    bool operator!=(const JsonValue& other) const;

    Type                    GetType() const;
    
    /// It is only valid to call this method if GetType returns Number
    NumericType				GetNumericType() const;

    /// throws MismatchedType if the type isn't type_Map
    const JsonValueMap&     GetMap() const;

    /// throws MismatchedType if the type isn't type_Map
    JsonValueMap&           GetMap();

    /// throws MismatchedType if the type isn't type_List
    const JsonValueList&    GetList() const;

    /// throws MismatchedType if the type isn't type_List
    JsonValueList&          GetList();

    /// throws MismatchedType if the type isn't type_String
    const std::string&      GetString() const;
    std::string&            GetString();

    double                  GetNumber() const;
    int32_t                 GetInt32() const;
    uint32_t                GetUInt32() const;
    int64_t                 GetInt64() const;

    /// throws MismatchedType if the type isn't type_Bool
    bool                    GetBool() const;

    void SetMap(const JsonValueMap& inMap);
    void SetMap(JsonValueMap&& inMap);

    void SetList(const JsonValueList& inList);
    void SetList(JsonValueList&& inList);

    void SetString(const std::string& inStr);
    void SetString(std::string&& inStr);

    void SetNumber(double inValue);
    void SetNumber(int32_t inValue);
    void SetNumber(uint32_t inValue);
    void SetNumber(int64_t inValue);

    void SetBool(bool inBool);

    void SetNull();

    void Swap(JsonValue& other);

    void Clear();
    void Copy(const JsonValue& rhs);

private:

	template <typename T>
	T GetNumericValue() const;

    union Value
    {
    	explicit constexpr Value() :
    		mMap(nullptr)
		{ }

    	explicit constexpr Value(bool value) :
    		mBoolean(value)
		{ }

        explicit constexpr Value(double value) :
            mDouble(value)
        { }

        explicit constexpr Value(int32_t value) :
            mInt32(value)
        { }

        explicit constexpr Value(uint32_t value) :
            mUInt32(value)
        { }

        explicit constexpr Value(int64_t value) :
            mInt64(value)
        { }

        JsonValueMap*   	mMap;
        JsonValueList*  	mList;
        std::string*    	mString;
		bool                mBoolean;
		double              mDouble;
		int32_t             mInt32;
		uint32_t            mUInt32;
		int64_t             mInt64;
    } mValue;
    
    Type 		mType;
	NumericType	mNumericType;
};

inline JsonValue::Type JsonValue::GetType() const
{
    return mType;
}

inline JsonValue::NumericType JsonValue::GetNumericType() const
{
    return mNumericType;
}

template <typename T>
T JsonValue::GetNumericValue() const
{
	switch (mNumericType)
	{
		case NumericType::Double:	return static_cast<T>(mValue.mDouble);
		case NumericType::Int32: 	return static_cast<T>(mValue.mInt32);
		case NumericType::UInt32: 	return static_cast<T>(mValue.mUInt32);
		case NumericType::Int64: 	return static_cast<T>(mValue.mInt64);
		case NumericType::Invalid:
			{
				return 0;
			}
			break;
	}
	// never reached (but VS2017 complains)
	return 0;
}

class Exception: public std::exception
{
public:
    virtual const char* what() const throw();
    Exception(std::string message) throw();
    virtual ~Exception() throw();
    
private:
    std::string mMessage;
};
    
class MismatchedType : public Exception
{
public:
    MismatchedType(std::string message) :
        Exception(message)
    { }
};

JsonValueMap::iterator
Find(   JsonValue& jsonValue,
        const std::string& key);

JsonValueMap::const_iterator
Find(   const JsonValue& jsonValue,
        const std::string& key);

/// Override to protect against automatic type conversion
JsonValueMap::iterator
Find(   JsonValueMap& jsonValueMap,
        const std::string& key);

/// Override to protect against automatic type conversion
JsonValueMap::const_iterator
Find(   const JsonValueMap& jsonValueMap,
        const std::string& key);


void swap(JsonValue& v1, JsonValue& v2);

}   // namespace jsonhandler


#endif // JSONHANDLER_JSON_VALUE_HPP
