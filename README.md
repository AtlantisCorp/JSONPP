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

## Serialization
You can serialize a structure with the serialize operator `<<`. The only thing to do is to provide a function 
`void toJSON(JSON&) const` in your structure, that will be used by `JSON::Value` to serialize the structure to a JSON object. 

```c++
struct T1
{
    int a;
    std::string b;
    
    inline void toJSON(JSON& rhs) const {
        rhs["a"] << a;
        rhs["b"] << b;
    }
};

mainObj["T1"] << T1{ 26, "hello elisa" };
```

With this simple principle, you can use nested serialization for other strutures: 

```c++
struct T2
{
    int a;
    T1 b;
    
    inline void toJSON(JSON& rhs) const {
        rhs["a"] << a;
        rhs["b"] << b;
    }
};
```

*Deserialization* follows the same principle, instead the function `void fromJSON(const JSON&)` is called when using the 
`>>` operator. 

## Build instructions
Nothing special needed. Just run `cmake` with your desired generator and compile the library. Just change SHARED to STATIC if 
you want a STATIC library instead. 
