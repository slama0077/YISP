#include <iostream>
#include <string>
#include  "Reader.h"
#include "Printer.h"



Value* READ(std::string input) 
{
    return Reader::read_str(input);
    // return input;
    // Tokenizer tokenizer = Tokenizer(input);
    // while(true)
    // {
    //     auto token = tokenizer.next();
    //     if (!token) break;  // Exit the loop if `next()` returns an empty optional
    //     std::cout << "[" << *token << "]\n";  // Dereference to access the value
    // }
    // return input;
}



Value* EVAL(Value* input)
{
    return input;
}

std::string PRINT(Value* input)
{
    return pr_str(input);
}

std::string rep(std::string input)
{
    auto ast = READ(input);
    auto result = EVAL(ast);
    return PRINT(result);
}

int main()
{
    std::string input;

    while(1)
    {
        std::cout << "user (CTRL + C to exit)> ";
        std::getline(std::cin, input);
        std::cout << rep(input) << std::endl;
    }   
    return 0;
}