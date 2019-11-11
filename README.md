# JSONPP
A JSON Parser and Manipulator written in C++.

This project is just a try to do a simple parser in C++ for the JSON file format. Using it is as simple as:

```c++
#include <JSONPP.h>

int main() 
{
    JSON mainObj = JSON::load("Path/to/your/file.json");
    std::cout << mainObj << std::endl;
}
```

Basically, the main JSON object is a list of JSON Values. A Value is a pair of a string name and a possible value between
a boolean, a string, a number, an array or an other object. So you can access your objects as simply as: 

```c++
std::string str = mainObj["value1"].toObject()["subvalue2"];
```

If any of the above expression is invalid, a JSON::Exception object is thrown. To avoid those exceptions and get a valid
Value, you can use instead `JSON::valueOf()` which returns a default value if the value is not found. 

## Build instructions
Nothing special needed. Just run `cmake` with your desired generator and compile the library. Just change SHARED to STATIC if 
you want a STATIC library instead. 
