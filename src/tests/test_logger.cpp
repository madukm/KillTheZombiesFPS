#include <logger.hpp>

// Fibonacci series
long fibonacci(unsigned n) 
{ 
    if (n < 2) return n; 
    return fibonacci(n-1) + fibonacci(n-2); 
} 

int main()
{
    char buf[256];
    logger a_logger;
    
    for (int i = 0 ; i < 30 ; i++)
    {
        snprintf(buf, 256, "%d", fibonacci(i));
        a_logger << std::string(buf);
    }
    
    return 0;
}

/*
// C++ program to illustrate time point 
// and system clock functions 
#include <iostream> 
#include <chrono> 
#include <ctime> 
*/
