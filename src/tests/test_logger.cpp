#include "../shared/log/logger.hpp"

// Fibonacci series
long fibonacci(unsigned n) 
{ 
    if (n < 2) return n; 
    return fibonacci(n-1) + fibonacci(n-2); 
} 

int main()
{
    char buf[256];
    logger::logger a_logger(std::string("Fib"));
    
    for (int i = 0 ; i < 50 ; i++)
    {
        snprintf(buf, 256, "%ld", fibonacci(i));
        a_logger.log(buf, logger::log_type::NORM);
    }
    
    return 0;
}

