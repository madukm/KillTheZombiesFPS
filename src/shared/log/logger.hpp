#include <iostream>
#include <cstdio>
#include <chrono>

class logger
{
    public:
    
    logger(std::string logger_name)
    {
        start = std::chrono::system_clock::now();
        name = std::string(logger_name);
    }
    
    operator<<(std::string message)
    {
        std::chrono::time_point<std::chrono::system_clock> now;
        now = std::chrono::system_clock::now();
        
        std::chrono::duration<double> time_diff = now - start;
        
        printf("[%s] %lf: %s",
               logger_name.c_str(),
               time_diff.count(),
               message.c_str());
    }
    
    private:
    
    std::string name;
    std::chrono::time_point<std::chrono::system_clock> start;
}

