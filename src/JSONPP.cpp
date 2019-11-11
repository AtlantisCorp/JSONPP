// JSONPP.cpp
// The simple C++ JSON Parser. 
// Copyright 2019 @Atlanti's Corp

#include "JSONPP.h"

#include <cstdarg>
#include <cstdio>
#include <algorithm>
#include <sstream>

JSON::Value JSON::NullValue = JSON::Value();

JSON::Exception::Exception(const char* format, ...)
{
	char buffer[JSON_MAX_BUFFER];
	va_list arg;
	va_start(arg, format);
	vsnprintf(buffer, JSON_MAX_BUFFER, format, arg);
	va_end(arg);
	mBuffer = std::string(buffer);
}

const char* JSON::Exception::what() const noexcept
{
	return mBuffer.data();
}

JSON::Value::Value(const std::string& name)
{
	mName = name;
	mType = T_NULL;
}

JSON::Value::Value(const std::string& name, const JSON& object)
{
	mName = name;
	mType = T_OBJECT;
    mObject.reset(new JSON(object));
}

JSON::Value::Value(const std::string& name, const JSON::Number& number)
{
	mName = name;
	mNumber = number;
	mType = T_NUMBER;
}

JSON::Value::Value(const std::string& name, const std::string& string)
{
	mName = name;
	mString = string;
	mType = T_STRING;
}

JSON::Value::Value(const std::string& name, const std::vector < Value >& array)
{
	mName = name;
	mArray = array;
	mType = T_ARRAY;
}

JSON::Value::Value(const std::string& name, bool rhs)
{
    mName = name;
    mBoolean = rhs;
    mType = T_BOOLEAN;
}

JSON::Value::Value(const JSON::Value& rhs)
{
	mName = rhs.mName;
	mType = rhs.mType;
	
	if (rhs.mObject)
	mObject.reset(new JSON(*rhs.mObject));
	
	mString = rhs.mString;
	mNumber = rhs.mNumber;
	mArray = rhs.mArray;
    mBoolean = rhs.mBoolean;
}

JSON::Value& JSON::Value::operator=(const JSON::Value& rhs)
{
    mName = rhs.mName;
    mType = rhs.mType;
    
    if (rhs.mObject)
    mObject.reset(new JSON(*rhs.mObject));
    
    mString = rhs.mString;
    mNumber = rhs.mNumber;
    mArray = rhs.mArray;
    mBoolean = rhs.mBoolean;
    
    return *this;
}

bool JSON::Value::isObject() const 
{
	return mType == T_OBJECT;
}

bool JSON::Value::isNumber() const 
{
	return mType == T_NUMBER;
}

bool JSON::Value::isString() const 
{
	return mType == T_STRING;
}

bool JSON::Value::isArray() const 
{
	return mType == T_ARRAY;
}

bool JSON::Value::isBoolean() const
{
    return mType == T_BOOLEAN;
}

bool JSON::Value::isNull() const 
{
	return mType == T_NULL;
}

JSON& JSON::Value::toObject() 
{
	JSON_THROW_IF(!isObject(), Exception("invalid toObject() type conversion."));
	JSON_THROW_IF(!mObject, Exception("null object held."));
	return *mObject;
}

const JSON& JSON::Value::toObject() const 
{
	JSON_THROW_IF(!isObject(), Exception("invalid toObject() type conversion."));
	JSON_THROW_IF(!mObject, Exception("null object held."));
	return *mObject;
}

JSON::Number& JSON::Value::toNumber()
{
	JSON_THROW_IF(!isNumber(), Exception("invalid toNumber() type conversion."));
	return mNumber;
}

const JSON::Number& JSON::Value::toNumber() const
{
	JSON_THROW_IF(!isNumber(), Exception("invalid toNumber() type conversion."));
	return mNumber;
}

std::string& JSON::Value::toString() 
{
	JSON_THROW_IF(!isString(), Exception("invalid toString() type conversion."));
	return mString;
}

const std::string& JSON::Value::toString() const
{
	JSON_THROW_IF(!isString(), Exception("invalid toString() type conversion."));
	return mString;
}

JSON::Array& JSON::Value::toArray() 
{
	JSON_THROW_IF(!isArray(), Exception("invalid toArray() type conversion."));
	return mArray;
}

const JSON::Array& JSON::Value::toArray() const
{
	JSON_THROW_IF(!isArray(), Exception("invalid toArray() type conversion."));
	return mArray;
}

bool& JSON::Value::toBoolean()
{
    JSON_THROW_IF(!isBoolean(), Exception("invalid toBoolean() type conversion."));
    return mBoolean;
}

const bool& JSON::Value::toBoolean() const
{
    JSON_THROW_IF(!isBoolean(), Exception("invalid toBoolean() type conversion."));
    return mBoolean;
}

const std::string& JSON::Value::name() const 
{
	return mName;
}

JSON::Value& JSON::Value::at(const std::size_t& index) 
{
	JSON_THROW_IF(!isArray(), Exception("invalid Array type conversion."));
	return mArray.at(index);
}

const JSON::Value& JSON::Value::at(const std::size_t& index) const
{
	JSON_THROW_IF(!isArray(), Exception("invalid Array type conversion."));
	return mArray.at(index);
}

std::string JSON::Value::str(unsigned level, bool printName) const
{
    std::string tabulates;
    for (unsigned i = 0; i < level; ++i)
        tabulates += "\t";
    
    std::string result;
    std::stringstream stream(result);
    
    if(printName)
    stream << tabulates << "\"" << mName << "\"" << ":";
    else
    stream << tabulates;
    
    if (isObject())
    {
        stream << std::endl;
        stream << mObject->str(level);
    }
    
    else if (isArray())
    {
        stream << " [" << std::endl;
        
        for (unsigned i = 0; i < mArray.size(); ++i)
        {
            stream << mArray.at(i).str(level + 1, false);
            
            if (i + 1 < mArray.size())
                stream << ",";
            
            stream << std::endl;
        }
        
        stream << tabulates << "]";
    }
    
    else if (isNumber())
    {
        stream << " " << mNumber;
    }
    
    else if (isString())
    {
        stream << " \"" << mString << "\"";
    }
    
    else if (isBoolean())
    {
        if (mBoolean)
            stream << " true";
        else
            stream << " false";
    }
    
    return stream.str();
}

JSON::Value& JSON::Value::operator=(const std::string &rhs)
{
    if (isObject())
        mObject.reset();
    mString = rhs;
    mType = T_STRING;
    return *this;
}

JSON::Value& JSON::Value::operator=(const JSON::Number& rhs)
{
    if (isObject())
        mObject.reset();
    mNumber = rhs;
    mType = T_NUMBER;
    return *this;
}

JSON::Value& JSON::Value::operator=(const JSON::Array& rhs)
{
    if (isObject())
        mObject.reset();
    mArray = rhs;
    mType = T_ARRAY;
    return *this;
}

JSON::Value& JSON::Value::operator=(const JSON& rhs)
{
    mObject.reset(new JSON(rhs));
    mType = T_OBJECT;
    return *this;
}

JSON::Value& JSON::Value::operator=(bool rhs)
{
    if (isObject())
        mObject.reset();
    mBoolean = rhs;
    mType = T_BOOLEAN;
    return *this;
}

JSON::Value::operator std::string()
{
    return toString();
}

JSON::Value::operator const std::string() const
{
    return toString();
}

JSON::Value::operator JSON::Number()
{
    return toNumber();
}

JSON::Value::operator const JSON::Number() const
{
    return toNumber();
}

JSON::Value::operator JSON::Array()
{
    return toArray();
}

JSON::Value::operator const JSON::Array() const
{
    return toArray();
}

JSON::Value::operator JSON()
{
    return toObject();
}

JSON::Value::operator const JSON() const
{
    return toObject();
}

JSON::Value::operator bool()
{
    return toBoolean();
}

JSON::Value::operator const bool() const
{
    return toBoolean();
}

JSON::Value& JSON::value(const std::string& name) 
{
	auto iter = std::find_if(mValues.begin(), mValues.end(), [name](auto& rhs){
		return rhs.name() == name;
	});
	
	if (iter == mValues.end())
	return NullValue;
	
	return *iter;
}

const JSON::Value& JSON::value(const std::string& name) const
{
	auto iter = std::find_if(mValues.begin(), mValues.end(), [name](auto& rhs){
		return rhs.name() == name;
	});
	
	if (iter == mValues.end())
	return NullValue;
	
	return *iter;
}

const std::string& JSON::valueOf(const std::string& name, const std::string& def) const
{
	const Value& result = value(name);
	
	if (result.isNull())
	return def;
	
	return result.toString();
}

const JSON::Number& JSON::valueOf(const std::string& name, const JSON::Number& def) const
{
	const Value& val = value(name);
	
	if (val.isNull())
	return def;
	
	return val.toNumber();
}

const JSON& JSON::valueOf(const std::string& name, const JSON& def) const
{
	const Value& val = value(name);
	
	if (val.isNull())
	return def;
	
	return val.toObject();
}

const JSON::Array& JSON::valueOf(const std::string& name, const JSON::Array& def) const 
{
	const Value& val = value(name);
	
	if (val.isNull())
	return def;
	
	return val.toArray();
}

JSON::Value& JSON::operator[](const std::string& name)
{
	Value& result = value(name);
    
    if (result.isNull())
    {
        addValue(Value(name));
        return value(name);
    }
    
    return result;
}

const JSON::Value& JSON::operator[](const std::string& name) const
{
	return value(name);
}

JSON::Array::iterator JSON::begin() 
{
	return mValues.begin();
}

JSON::Array::const_iterator JSON::begin() const
{
	return mValues.begin();
}

JSON::Array::const_iterator JSON::cbegin() const
{
	return mValues.cbegin();
}

JSON::Array::iterator JSON::end() 
{
	return mValues.end();
}

JSON::Array::const_iterator JSON::end() const
{
	return mValues.end();
}

JSON::Array::const_iterator JSON::cend() const
{
	return mValues.cend();
}

void JSON::addValue(const JSON::Value& rhs) 
{
	Value& check = value(rhs.name());
	
	if (!check.isNull())
	check = rhs;
	
	else 
	mValues.push_back(rhs);
}

std::string JSON::str(unsigned level) const
{
    std::string tabulates;
    for (unsigned i = 0; i < level; ++i)
        tabulates += "\t";
    
    std::string result;
    std::stringstream stream(result);
    stream << tabulates << "{" << std::endl;
    
    for (int i = 0; i < mValues.size(); ++i)
    {
        stream << mValues.at(i).str(level + 1);
        
        if (i + 1 < mValues.size())
            stream << ",";
        
        stream << std::endl;
    }
    
    stream << tabulates << "}";
    return stream.str();
}

std::ostream& operator << (std::ostream& out, const JSON& obj)
{
    out << obj.str();
    return out;
}

std::ostream& operator << (std::ostream& out, const JSON::Value& value)
{
    out << value.str();
    return out;
}
