#ifndef ERROR_H
#define ERROR_H

#include <iostream>
#include <string>

struct ErrorMessage
{
    unsigned int code;
    std::string message;

    ErrorMessage(int code, std::string message) : code(code), message(message) {}
};

class Error
{
public:
    static const ErrorMessage CANT_LOAD_MODEL;

    static void throwError(const ErrorMessage& message, std::string info)
    {
        
        std::cerr << "ERROR:: " << message.code << ": " << message.message;
        if (info.length() > 0) 
        {
            std::cerr << std::endl << info;
        }
        std::cerr << std::endl;

        exit(message.code);
    }
    
    static void ThrowWarning(const ErrorMessage& message, std::string info)
    {
    
        std::cerr << "WARNING:: " << message.code << ": " << message.message;
        if (info.length() > 0) 
        {
            std::cerr << std::endl << info;
        }
        std::cerr << std::endl;
    }
};


const ErrorMessage Error::CANT_LOAD_MODEL(0, "Unable to load model");
#endif