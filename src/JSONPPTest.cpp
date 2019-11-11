// JSONPPTest.cpp
// Test program for JSONPP Library.

#include "JSONPP.h"

#include <iostream>
#include <chrono>

int main(int argc, char** argv)
{
    try
    {
        JSON mainObj = JSON::loadContent("{ \"a\": 1.0, \"b\": \"hello world!\" }");
        std::cout << mainObj << std::endl;
        
        mainObj["b"] = (JSON::Number)3.0;
        std::cout << mainObj << std::endl;
        
        mainObj = JSON::load("AMakeFile.json");
        std::cout << mainObj << std::endl;
        
        using namespace std::chrono;
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        
        for (unsigned i = 0; i < 1000; ++i)
            mainObj = JSON::load("AMakeFile.json");
        
        auto duration = high_resolution_clock::now() - t1;
        std::cout << "Total loading: " << duration_cast<microseconds>(duration).count() << "us." << std::endl;
        std::cout << "Per file time: " << duration_cast<microseconds>(duration).count() / 1000 << "us." << std::endl;
        
        mainObj = JSON::loadContent("{ \"ab\": true }");
        std::cout << mainObj << std::endl;
        
        return 0;
    }
    
    catch(const JSON::Exception& e)
    {
        std::cerr << "caught exception: " << e.what() << std::endl;
        return -1;
    }
    
    
}
