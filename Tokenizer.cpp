// tokenizer.cpp
#include "Tokenizer.h"
#include <iostream>

// Constructor implementation
Tokenizer::Tokenizer(std::string input) : input(std::move(input)) {}

// `next` function implementation
std::optional<std::string> Tokenizer::next()
{
    while (index < input.length())
    {
        char c = input[index];
        int start;

        switch (c)
        {
        case ' ':
        case '\t':
        case '\n':
        case ',':
            index++; // Skip whitespace and commas
            break;
        case '~':
            index++;
            if (index < input.length() && input[index] == '@')
            {
                index++;
                return input.substr(index - 2, 2); // Handle ~@ token
            }
            return input.substr(index - 1, 1); // Handle ~ token
        case '[':
        case ']':
        case '{':
        case '}':
        case '(':
        case ')':
        case '\'':
        case '`':
        case '^':
        case '@':
            return input.substr(index++, 1); // Return single character tokens
        case '"':
        {
            start = index;
            index++; // Skip opening quote
            while (index < input.length())
            {
                c = input[index];
                if (c == '"')
                {
                    index++; // Skip closing quote
                    return input.substr(start, index - start);
                }
                if (c == '\\')
                { // Handle escape sequences
                    index++;
                }
                index++;
            }
            std::cout << "EOF, will be an exception\n"; // Handle unterminated string case
            return input.substr(start, index - start);
        }
        case ';':
        {
            start = index;
            while (index < input.length() && input[index] != '\n')
            {
                index++; // Skip comment until newline
            }
            return input.substr(start, index - start); // Return entire comment
        }
        case '-':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        {
            size_t start = index;
            bool done = false;
            ++index;
            while (!done && index < input.length())
            {
                c = input[index];
                switch (c)
                {
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                    ++index;
                    break;
                default:
                    done = true;
                    break;
                }
            }
            if (index - start == 1 && input.at(start) == '-')
            {
                --index;
                return tokenize_symbol();
            }
            return input.substr(start, index - start); // Return a toke
        }
        default:
        {
            return tokenize_symbol();
        }
        }
    }
    return std::nullopt; // Return nullopt to indicate no more tokens
}

std::string Tokenizer::tokenize_symbol()
{
    size_t start = index;
    bool done = false;
    char c;
    while (!done && index < input.length())
    {
        c = input[index];
        switch (c)
        {
        case ' ':
        case '\t':
        case '\n':
        case ',':
        case '[':
        case ']':
        case '{':
        case '}':
        case '(':
        case ')':
        case '\'':
        case '"':
        case '`':
        case ';':
            done = true;
            break;
        default:
            index++;
        }
    }
    return input.substr(start, index - start); // Return a token
}
