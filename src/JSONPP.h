// JSONPP.h
// The simple C++ JSON Parser. 
// Copyright 2019 @Atlanti's Corp

#include <exception>
#include <string>
#include <memory>
#include <vector>
#include <iostream>

#define JSON_THROW(EXC) throw EXC
#define JSON_THROW_IF(CND, EXC) if((CND)) JSON_THROW(EXC)
#define JSON_MAX_BUFFER 4096

//! @brief The Main JSON Object class.
//! This class has multiple purpose: being the Object JSON structure, represents
//! the main JSON file (the main object), and holds all the JSON other datas.
class JSON 
{
public:
	//! @brief The Number representation. 
	typedef float Number;
	
	//! @brief An Exception for all JSON functions.
	class Exception : public std::exception 
	{
		//! @brief Stores the what() answer.
		std::string mBuffer;
	public:
		//! @brief Constructs a new Exception in a printf-like manner.
		Exception(const char* format, ...);
		//! @brief Returns the message.
		const char* what() const noexcept;
	};
	
	//! @brief A JSON Value is a pair of a name and: an object, a number or a string.
	class Value 
	{
		//! @brief Holds the name of this value.
		std::string mName;
		
		//! @brief Holds an object if this is an object.
		std::unique_ptr < JSON > mObject;
		
		//! @brief Holds a number if this is a number.
		Number mNumber;
		
		//! @brief Holds a string if this is a string.
		std::string mString;
		
		//! @brief Holds an array of values if this is an array.
		std::vector < Value > mArray;
		
		enum Type { T_OBJECT, T_NUMBER, T_STRING, T_ARRAY, T_BOOLEAN, T_NULL };
		//! @brief Holds the type of this value.
		Type mType;
        
        //! @brief Holds the bool value, true or false, if T_BOOLEAN.
        bool mBoolean;
		
	public:
		//! @brief Constructs a Value with a null type.
		Value(const std::string& name = std::string());
		//! @brief Constructs a Value from an object.
		Value(const std::string& name, const JSON& object);
		//! @brief Constructs a Value from a number.
		Value(const std::string& name, const Number& number);
		//! @brief Constructs a Value from a string.
		Value(const std::string& name, const std::string& string);
		//! @brief Constructs a Value from an array.
		Value(const std::string& name, const std::vector < Value >& array);
        //! @brief Constructs a Value from a boolean.
        Value(const std::string& name, bool rhs);
		//! @brief Copies the value.
		Value(const Value& rhs);
        
        //! @brief Copies the value.
        Value& operator = (const Value& rhs);
		
		//! @brief Returns true if this value is an object.
		bool isObject() const;
		//! @brief Returns true if this value is a number.
		bool isNumber() const;
		//! @brief Returns true if this value is a string.
		bool isString() const;
		//! @brief Returns true if this value is an array.
		bool isArray() const;
        //! @brief Returns true if this value is a boolean.
        bool isBoolean() const;
		//! @brief Returns true if this value is a null value.
		bool isNull() const;
		
		//! @brief Returns a reference to the object, or throws an exception 
		//! if this value doesn't hold an object.
		JSON& toObject();
		//! @brief Returns a reference to the object, or throws an exception 
		//! if this value doesn't hold an object.
		const JSON& toObject() const;
		
		//! @brief Returns a reference to the number, or throws an exception 
		//! if this value doesn't hold a number.
		Number& toNumber();
		//! @brief Returns a reference to the number, or throws an exception 
		//! if this value doesn't hold a number.
		const Number& toNumber() const;
		
		//! @brief Returns a reference to the string, or throws an exception 
		//! if this value doesn't hold a string.
		std::string& toString();
		//! @brief Returns a reference to the string, or throws an exception 
		//! if this value doesn't hold a string.
		const std::string& toString() const;
		
		//! @brief Returns a reference to the array, or throws an exception 
		//! if this value doesn't hold a array.
		std::vector < Value >& toArray();
		//! @brief Returns a reference to the array, or throws an exception 
		//! if this value doesn't hold a array.
		const std::vector < Value >& toArray() const;
        
        //! @brief Returns a reference to the boolean, or throws an exception
        //! if this value doesn't hold a boolean.
        bool& toBoolean();
        //! @brief Returns a reference to the boolean, or throws an exception
        //! if this value doesn't hold a boolean.
        const bool& toBoolean() const;
		
		//! @brief Returns the name of this value.
		const std::string& name() const;
		
		//! @brief Returns the value at given index if this value is an Array. 
		Value& at(const std::size_t& index);
		//! @brief Returns the value at given index if this value is an Array. 
		const Value& at(const std::size_t& index) const;
        
        //! @brief Writes the JSON Value into a string.
        //! @param level The number of tabulation to insert.
        //! @param printName Boolean false if we shouldn't print name (specific to Arrays).
        std::string str(unsigned level = 0, bool printName = true) const;
        
        //! @brief Changes the Value to a string.
        Value& operator=(const std::string& rhs);
        //! @brief Changes the Value to a number.
        Value& operator=(const Number& rhs);
        //! @brief Changes the Value to an array.
        Value& operator=(const std::vector < Value >& rhs);
        //! @brief Changes the Value to an object.
        Value& operator=(const JSON& rhs);
        //! @brief Changes the Value to a boolean.
        Value& operator=(bool rhs);
        
        //! @brief Tries to convert this value to a string.
        operator std::string();
        //! @brief Tries to convert this value to a string.
        operator const std::string() const;
        //! @brief Tries to convert this value to a number.
        operator Number();
        //! @brief Tries to convert this value to a number.
        operator const Number() const;
        //! @brief Tries to convert this value to an array.
        operator std::vector < Value >();
        //! @brief Tries to convert this value to an array.
        operator const std::vector < Value >() const;
        //! @brief Tries to convert this value to an object.
        operator JSON();
        //! @brief Tries to convert this value to an object.
        operator const JSON() const;
        //! @brief Tries to convert this value to a boolean.
        operator bool();
        //! @brief Tries to convert this value to a boolean.
        operator const bool() const;
	};
	
	//! @brief The Array representation.
	typedef std::vector < Value > Array;
	
	//! @brief The values in this object.
	Array mValues;
	
public:
	
	//! @brief Constructs a new JSON object. 
	JSON() = default;
	//! @brief Copies the JSON object.
	JSON(const JSON&) = default;
	
	//! @brief Returns the value with specified name.
	//! If this value doesn't exist, a NullValue is returned. 
	Value& value(const std::string& name);
	//! @brief Returns the value with specified name.
	//! If this value doesn't exist, a NullValue is returned. 
	const Value& value(const std::string& name) const;
	
	//! @brief Returns the converted value if existing.
	//! @param name The name of the value to find and retrieve. 
	//! @param def The default value returned if not found.
	const std::string& valueOf(const std::string& name, const std::string& def) const;
	//! @brief Returns the converted value if existing.
	//! @param name The name of the value to find and retrieve. 
	//! @param def The default value returned if not found.
	const Number& valueOf(const std::string& name, const Number& def) const;
	//! @brief Returns the converted value if existing.
	//! @param name The name of the value to find and retrieve. 
	//! @param def The default value returned if not found.
	const JSON& valueOf(const std::string& name, const JSON& def) const;
	//! @brief Returns the converted value if existing.
	//! @param name The name of the value to find and retrieve. 
	//! @param def The default value returned if not found.
	const Array& valueOf(const std::string& name, const Array& def) const;
	
	//! @brief Returns the value with specified name.
	//! If this value doesn't exist, a NullValue is returned. 
	Value& operator[](const std::string& name);
	//! @brief Returns the value with specified name.
	//! If this value doesn't exist, a NullValue is returned. 
	const Value& operator[](const std::string& name) const;
	
	//! @brief Returns the first iterator in this object. 
	Array::iterator begin();
	//! @brief Returns the first iterator in this object. 
	Array::const_iterator begin() const;
	//! @brief Returns the first iterator in this object. 
	Array::const_iterator cbegin() const;
	
	//! @brief Returns the last iterator in this object. 
	Array::iterator end();
	//! @brief Returns the last iterator in this object. 
	Array::const_iterator end() const;
	//! @brief Returns the last iterator in this object. 
	Array::const_iterator cend() const;
	
	//! @brief Adds the specified Value. 
	void addValue(const Value& value);
    
    //! @brief Writes the JSON Object into a string.
    //! @param level The number of tabulation to insert.
    std::string str(unsigned level = 0) const;
	
	//! @brief The Null Value returned each time value() returns an invalid value.
	static Value NullValue;
	
public:
	//! @brief Loads the specified file from a texted JSON. 
	//! @param file The path to the file to load. 
	static JSON load(const std::string& file);
    
    //! @brief Loads the content as a texted JSON.
    static JSON loadContent(const std::string& content);
};

std::ostream& operator << (std::ostream& out, const JSON& obj);
std::ostream& operator << (std::ostream& out, const JSON::Value& value);
