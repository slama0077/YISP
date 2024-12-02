// Reader.h
#pragma once

#include <string>
#include <optional>
#include <vector>
#include "types.h"
#include "Tokenizer.h"

class Reader
{
private:
    std::vector<std::string> tokens;
    int index = 0;

public:
    // Constructor
    Reader(std::vector<std::string> tokens);

    // Member functions
    std::optional<std::string> next();
    std::optional<std::string> peek();

    // Static functions
    static std::vector<std::string> tokenize(std::string input);
    static Value *read_str(std::string input);
    static Value *read_form(Reader &reader);
    static ListValue *read_list(Reader &reader);
    static VectorValue *read_vector(Reader &reader);
    static Value *read_atom(Reader &reader);
    static Value *read_quote(Reader &reader);
    static HashMapValue *read_hash_map(Reader &reader);
    static Value *read_integer(Reader &reader);
};
