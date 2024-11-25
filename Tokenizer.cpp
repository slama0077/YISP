// tokenizer.cpp
#include "Tokenizer.h"
#include <iostream>

// Constructor implementation
Tokenizer::Tokenizer(std::string input) : input(std::move(input)) {}

// `next` function implementation
std::optional<std::string> Tokenizer::next() {
    while (index < input.length()) {
        char c = input[index];
        int start;

        switch (c) {
            case ' ':
            case '\t':
            case '\n':
            case ',':
                index++; // Skip whitespace and commas
                break;
            case '~':
                index++;
                if (index < input.length() && input[index] == '@') {
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
            case '"': {
                start = index;
                index++; // Skip opening quote
                while (index < input.length()) {
                    c = input[index];
                    if (c == '"') {
                        index++; // Skip closing quote
                        return input.substr(start, index - start);
                    }
                    if (c == '\\') { // Handle escape sequences
                        index++;
                    }
                    index++;
                }
                std::cout << "EOF, will be an exception\n"; // Handle unterminated string case
                return input.substr(start, index - start);
            }
            case ';': {
                start = index;
                while (index < input.length() && input[index] != '\n') {
                    index++; // Skip comment until newline
                }
                return input.substr(start, index - start); // Return entire comment
            }
            default: {
                start = index;
                bool done = false;
                while (!done && index < input.length()) {
                    c = input[index];
                    switch (c) {
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
        }
    }
    return std::nullopt; // Return nullopt to indicate no more tokens
}
