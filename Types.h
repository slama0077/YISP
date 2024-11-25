// types.h
#pragma once

#include <string>
#include <vector>
#include <cassert>

class Value {
public:
    virtual ~Value() = default;
    virtual std::string inspect();  // Defined in types.cpp
};

class ListValue : public Value {
public:
    ListValue();

    void push(Value* value);
    std::string inspect() override;

private:
    std::vector<Value*> listValue;
};

class SymbolValue : public Value {
public:
    explicit SymbolValue(std::string symbol);
    
    std::string str();
    std::string inspect() override;

private:
    std::string symbol;
};
