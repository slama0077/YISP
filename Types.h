// types.h
#pragma once

#include <string>
#include <vector>
#include <cassert>
#include <unordered_map>



class Value {
public:
    virtual ~Value() = default;
    virtual std::string inspect();  // Defined in types.cpp
};

struct HashMapHash
{
    std::size_t operator() (Value* key) const noexcept
    {
        return std::hash<std::string> {} (key->inspect());
    }
};

struct HashMapPred
{
    constexpr bool operator() (Value* lhs, Value* rhs) const
    {
        return lhs==rhs;
    }
};

class ListValue : public Value {
public:
    ListValue();

    void push(Value* value);
    std::string inspect() override;

protected:
    std::vector<Value*> listValue;
};

class VectorValue : public ListValue {

    public:
    VectorValue();
    std::string inspect() override;
};

class HashMapValue: public Value
{
    public:
    HashMapValue();
    std::string inspect();
    void set(Value* key, Value* val);
    Value* get(Value* key);

    private:
    std::unordered_map<Value*, Value*, HashMapHash, HashMapPred> map;
};

class SymbolValue : public Value {
public:
    explicit SymbolValue(std::string symbol);
    
    std::string str();
    std::string inspect() override;

private:
    std::string symbol;
};
