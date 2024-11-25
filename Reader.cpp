// reader.cpp
#include "Reader.h"
#include <iostream>
#include "Types.h"
#include "Tokenizer.h"

// Constructor
Reader::Reader(std::vector<std::string> tokens) : tokens(std::move(tokens)), index(0) {}

// Member functions
std::optional<std::string> Reader::next() {
    if (index < tokens.size()) {
        return tokens.at(index++);
    }
    return std::nullopt;
}

std::optional<std::string> Reader::peek() {
    if (index < tokens.size()) {
        return tokens.at(index);
    }
    return std::nullopt;
}

// Static functions
std::vector<std::string> Reader::tokenize(std::string input) {
    Tokenizer tokenizer = Tokenizer(input);
    std::vector<std::string> tokenizer_vector;

    while (auto token = tokenizer.next()) {
        tokenizer_vector.push_back(*token);
    }
    return tokenizer_vector;
}

Value* Reader::read_str(std::string input) {
    auto tokens = tokenize(input);
    Reader reader = Reader(tokens);
    return read_form(reader);
}

Value* Reader::read_form(Reader& reader) {
    auto token = reader.peek();
    if (!token) {
        return nullptr;
    }
    switch (token.value()[0]) {
        case '(':
            return read_list(reader);
        default:
            return read_atom(reader);
    }
}

Value* Reader::read_list(Reader& reader) {
    reader.next();
    auto* list = new ListValue();
    while (auto token = reader.peek()) {
        if (*token == ")") {
            reader.next();
            break;
        }
        list->push(read_form(reader));
    }
    return list;
}

Value* Reader::read_atom(Reader& reader) {
    return new SymbolValue(*reader.next());
}