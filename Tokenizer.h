// tokenizer.h
#pragma once

#include <string>
#include <optional>
#include <vector>

class Tokenizer {
public:
    explicit Tokenizer(std::string input);

    std::optional<std::string> next();

private:
    std::string input;
    int index = 0;
};
