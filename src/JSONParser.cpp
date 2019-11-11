// JSONParser.cpp
// The simple C++ JSON Parser. 
// Copyright 2019 @Atlanti's Corp

#include "JSONPP.h"

#include <fstream>
#include <istream>

static std::size_t findNextCharacter(const std::string& content, std::size_t pos,
                                     std::size_t& currLine, std::size_t& currCol);
static std::string parseString(const std::string& content, std::size_t& pos, std::size_t& currLine,
                               std::size_t& currCol);
static JSON::Array readArray(const std::string& content, std::size_t& pos, std::size_t& currLine,
                             std::size_t& currCol);
static JSON::Number readNumber(const std::string& content, std::size_t& pos, std::size_t& currLine,
                               std::size_t& currCol);
static JSON::Value readValueContent(const std::string& name, const std::string& content,
                                    std::size_t& pos, std::size_t& currLine, std::size_t& currCol);
static JSON::Value readValue(const std::string& content, std::size_t& pos, std::size_t& currLine,                                      std::size_t& currCol);
static JSON readObject(const std::string& content, std::size_t& pos,
                       std::size_t& currLine, std::size_t& currCol);

static std::size_t findFirstCharacter(const std::string& content, std::size_t pos,
                                      std::size_t& currLine, std::size_t& currCol)
{
    while(std::isspace(content[pos]))
    {
        if (content[pos] == '\n')
        {
            currLine = currLine + 1;
            currCol = 0;
        }
        
        else
            currCol = currCol + 1;
        
        pos = pos + 1;
        
        JSON_THROW_IF(pos >= content.size(), JSON::Exception("Parser(%i:%i): no character found.",
                                                             currLine, currCol));
    }
    
    return pos;
}

static std::size_t findNextCharacter(const std::string& content, std::size_t pos, 
									 std::size_t& currLine, std::size_t& currCol)
{
    pos = pos + 1;
    currCol = currCol + 1;
	return findFirstCharacter(content, pos, currLine, currCol);
}

static std::string parseString(const std::string& content, std::size_t& pos, std::size_t& currLine,
							   std::size_t& currCol)
{
	JSON_THROW_IF(content[pos] != '"', JSON::Exception("Parser(%i:%i): expected '\"'.",
	currLine, currCol));
	
	std::string result;
	
	do 
	{
        pos = pos + 1;
        currCol = currCol + 1;
        
        JSON_THROW_IF(pos >= content.size(), JSON::Exception("Parser(%i:%i): unexpected EOF.",
        currLine, currCol));
        
        if (content[pos] == '"')
            break;
        
		result.push_back(content[pos]);

	} while(content[pos] != '"');
	
	return result;
}

static JSON::Array readArray(const std::string& content, std::size_t& pos, std::size_t& currLine,
						     std::size_t& currCol)
{
	JSON_THROW_IF(content[pos] != '[', JSON::Exception("Parser(%i:%i): expected '['.",
	currLine, currCol));
	
	JSON::Array array;
	
	while (content[pos] != ']')
	{
		pos = findNextCharacter(content, pos, currLine, currCol);
		
		if (pos == ']')
			break;
		
		JSON::Value value = readValueContent(std::to_string(array.size()), content, pos, currLine, currCol);
		array.push_back(value);
		
		pos = findNextCharacter(content, pos, currLine, currCol);
		
		JSON_THROW_IF(content[pos] != ',' && content[pos] != ']', 
		JSON::Exception("Parser(%i:%i): unexpected character %c.",
		currLine, currCol, content[pos]));
	}
	
	return array;
}

static JSON::Number readNumber(const std::string& content, std::size_t& pos, std::size_t& currLine,
						       std::size_t& currCol)
{
	bool isNeg = content[pos] == '-';
	bool hasPoint = false;
	
	JSON_THROW_IF(!isNeg && !std::isdigit(content[pos]),
	JSON::Exception("Parser(%i:%i): unexepected character %c.",
	currLine, currCol, content[pos]));
	
	std::string alnum;
	if (isNeg) alnum.push_back('-');
    alnum.push_back(content[pos]);
	
	while (content[pos] != ',')
	{
		pos = pos + 1;
        currCol = currCol + 1;
		
		if (std::isdigit(content[pos]))
			alnum.push_back(content[pos]);
		
		else if (content[pos] == '.')
		{
			JSON_THROW_IF(hasPoint, JSON::Exception("Parser(%i:%i): unexpected floating point.",
			currLine, currCol));
			
			alnum.push_back(content[pos]);
			hasPoint = true;
		}
        
        else if(content[pos] != ',')
        {
            JSON_THROW(JSON::Exception("Parser(%i:%i): unexepected character %c.",
            currLine, currCol, content[pos]));
        }
		
		JSON_THROW_IF(pos >= content.size(), JSON::Exception("Parser(%i:%i): unexpected EOF.",
		currLine, currCol));
	}
	
    pos = pos - 1;
    currCol = currCol - 1;
    return strtof(alnum.data(), NULL);
}

static bool readBoolean(const std::string& content, std::size_t& pos, std::size_t& currLine,
                        std::size_t& currCol)
{
    JSON_THROW_IF(content[pos] != 't' && content[pos] != 'f',
    JSON::Exception("Parser(%i:%i): expected character 't' or 'f'.",
    currLine, currCol));
    
    JSON_THROW_IF(pos + 3 >= content.size(),
    JSON::Exception("Parser(%i:%i): expected at least 4 characters.",
    currLine, currCol));
    
    if (content[pos]     == 't' &&
        content[pos + 1] == 'r' &&
        content[pos + 2] == 'u' &&
        content[pos + 3] == 'e')
    {
        pos = pos + 3;
        currCol = currCol + 3;
        return true;
    }
    
    JSON_THROW_IF(pos + 4 >= content.size(),
    JSON::Exception("Parser(%i:%i): expected at least 5 characters.",
    currLine, currCol));
    
    if (content[pos]     == 'f' &&
        content[pos + 1] == 'a' &&
        content[pos + 2] == 'l' &&
        content[pos + 3] == 's' &&
        content[pos + 4] == 'e')
    {
        pos = pos + 4;
        currCol = currCol + 4;
        return false;
    }
    
    JSON_THROW(JSON::Exception("Parser(%i:%i): unexpected character %c.",
    currLine, currCol, content[pos]));
    return false;
}

static JSON::Value readValueContent(const std::string& name, const std::string& content, 
									std::size_t& pos, std::size_t& currLine, std::size_t& currCol)
{
	if (content[pos] == '"')
	{
		std::string str = parseString(content, pos, currLine, currCol);
		return JSON::Value(name, str);
	}
	
	else if (content[pos] == '{')
	{
		JSON obj = readObject(content, pos, currLine, currCol);
		return JSON::Value(name, obj);
	}
	
	else if (content[pos] == '[')
	{
		JSON::Array array = readArray(content, pos, currLine, currCol);
		return JSON::Value(name, array);
	}
	
	else if (std::isdigit(content[pos]) || content[pos] == '-')
	{
		JSON::Number num = readNumber(content, pos, currLine, currCol);
		return JSON::Value(name, num);
	}
    
    else if (content[pos] == 't' || content[pos] == 'f')
    {
        bool b = readBoolean(content, pos, currLine, currCol);
        return JSON::Value(name, b);
    }
	
	JSON_THROW(JSON::Exception("Parser(%i:%i): unexpected character %c.",
	currLine, currCol, content[pos]));
	return JSON::Value();
}

static JSON::Value readValue(const std::string& content, std::size_t& pos, std::size_t& currLine, 							 std::size_t& currCol)
{
	JSON_THROW_IF(content[pos] != '"', JSON::Exception("Parser(%i:%i): key/value should begin with a string.",
	currLine, currCol));
	
	std::string key = parseString(content, pos, currLine, currCol);
	JSON_THROW_IF(key.empty(), JSON::Exception("Parser(%i:%i): key cannot be empty.",
	currLine, currCol));
	
	pos = findNextCharacter(content, pos, currLine, currCol);
	JSON_THROW_IF(content[pos] != ':', JSON::Exception("Parser(%i:%i): ':' was expected.",
	currLine, currCol));
	
	pos = findNextCharacter(content, pos, currLine, currCol);
	return readValueContent(key, content, pos, currLine, currCol);
}

static JSON readObject(const std::string& content, std::size_t& pos,
                       std::size_t& currLine, std::size_t& currCol)
{
	JSON_THROW_IF(content[pos] != '{', JSON::Exception("Parser(%i:%i): object should begin with a '{'.",
	currLine, currCol));
	
	JSON result;
	
	while(content[pos] != '}')
	{
		pos = findNextCharacter(content, pos, currLine, currCol);
		
		if (pos == '}')
		break;
		
		JSON::Value value = readValue(content, pos, currLine, currCol);
		result.addValue(value);
		
		pos = findNextCharacter(content, pos, currLine, currCol);
		
		JSON_THROW_IF(content[pos] != ',' && content[pos] != '}', 
		JSON::Exception("Parser(%i:%i): unexpected character %c.",
		currLine, currCol, content[pos]));
	};
    
    return result;
}

JSON JSON::load(const std::string& file)
{
	std::ifstream stream(file.data(), std::ios::in);
	JSON_THROW_IF(!stream, Exception("cannot open input file %s.", file.data()));
	
	std::string content((std::istreambuf_iterator<char>(stream)),
                 		 std::istreambuf_iterator<char>());
	
	stream.close();
	
	return loadContent(content);
}

JSON JSON::loadContent(const std::string &content)
{
    std::size_t currLine = 0, currCol = 0, currPos = 0;
    currPos = findFirstCharacter(content, currPos, currLine, currCol);
    JSON_THROW_IF(content[currPos] != '{', Exception("Parser(%i:%i): first character should be a '{'.",
    currLine, currCol));
    
    return readObject(content, currPos, currLine, currCol);
}
