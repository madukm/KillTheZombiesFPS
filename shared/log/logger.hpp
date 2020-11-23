#include <iostream>
#include <cstdio>
#include <chrono>

namespace logger
{
    enum log_type
    {
        NORM,
        WARN,
        DEBUG
    };
    
    class logger
    {
        public:
        inline static std::chrono::time_point<std::chrono::system_clock> start;
        
        public:
        
        logger(std::string logger_name)
        {
            start = std::chrono::system_clock::now();
            name = std::string(logger_name);
        }
        
        void log(std::string message, log_type t)
        {
            __set_color(t);
            __log(message);
            __unset_color();
        }
        
        void log(char* message, log_type t)
        {
            __set_color(t);
            __log(std::string(message));
            __unset_color();
        }
        
        void log(const char* message, log_type t)
        {
            __set_color(t);
            __log(std::string(message));
            __unset_color();
        }
        
        private:
        
        void __log(std::string message)
        {
            std::chrono::time_point<std::chrono::system_clock> now;
            now = std::chrono::system_clock::now();
            
            std::chrono::duration<double> time_diff = now - start;
            
            printf("[%s] %lf: %s\n",
                   name.c_str(),
                   time_diff.count(),
                   message.c_str());
        }
        
        void __set_color(log_type t)
        {
            switch(t)
            {
                case NORM:
                printf("\033[36m");
                break;
                
                case WARN:
                printf("\033[31m");
                break;
                
                case DEBUG:
                printf("\033[33m");
                break;
            }
        }
        
        void __unset_color()
        {
            printf("\033[37m");
        }
        
        std::string name;
    };
}
