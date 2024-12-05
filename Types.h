// types.h
#pragma once

#include <string>
#include <vector>
#include <cassert>
#include <unordered_map>

class ListValue;
class VectorValue;
class HashMapValue;
class SymbolValue;
class IntegerValue;
class FnValue;
class ExceptionValue;

class Value
{
public:
    enum class Type
    {
        List,
        Vector,
        HashMap,
        Symbol,
        Integer,
        Fn,
        Exception,
    };
    virtual ~Value() = default;
    virtual std::string inspect() = 0; // Defined in types.cpp
    virtual bool is_symbol() {return false; };
    virtual Type type() = 0;
    ListValue *as_list();
    VectorValue *as_vector();
    HashMapValue *as_hash_map();
    SymbolValue *as_symbol();
    IntegerValue *as_integer();
    FnValue *as_fn();
    ExceptionValue *as_exception();
};

struct HashMapHash
{
    std::size_t operator()(Value *key) const noexcept
    {
        return std::hash<std::string>{}(key->inspect());
    }
};

struct HashMapPred
{
    bool operator()(Value *lhs, Value *rhs) const
    {
        return lhs->inspect() == rhs->inspect();
    }
};

class ListValue : public Value
{
public:
    ListValue();

    void push(Value *value);
    std::string inspect() override;
    virtual Type type() { return Type::List; }
    auto begin() { return listValue.begin(); }
    auto end() { return listValue.end(); }
    bool is_empty() { return listValue.size() == 0; }
    size_t size() { return listValue.size(); }
    Value *at(size_t index) { return listValue[index]; }

protected:
    std::vector<Value *> listValue;
};

class VectorValue : public ListValue
{

public:
    VectorValue();
    std::string inspect() override;
    virtual Type type() { return Type::Vector; }
};

class HashMapValue : public Value
{
public:
    HashMapValue();
    std::string inspect();
    void set(Value *key, Value *val);
    Value *get(Value *key);
    virtual Type type() { return Type::HashMap; }
    auto begin() { return map.begin(); }
    auto end() { return map.end(); }

private:
    std::unordered_map<Value *, Value *, HashMapHash, HashMapPred> map;
};

class SymbolValue : public Value
{
public:
    explicit SymbolValue(std::string symbol);

    bool matches(char *nstr) {return symbol == nstr;}

    std::string str();
    std::string inspect() override;
    virtual Type type() { return Type::Symbol; }
    virtual bool is_symbol() override {return true;}
private:
    std::string symbol;
};

using FnPtr = Value *(*)(size_t, Value **);

class IntegerValue : public Value
{
public:
    IntegerValue(long l)
        : m_long{l} {}

    virtual std::string inspect()
    {
        return std::to_string(m_long);
    }
    long to_long() { return m_long; }
    virtual Type type() { return Type::Integer; }

private:
    long m_long{0};
};

class FnValue : public Value
{
public:
    FnValue(FnPtr fn)
        : m_fn{fn} {}
    virtual std::string inspect()
    {
        return "<fn>";
    }
    FnPtr to_fn() { return m_fn; }
    virtual Type type() { return Type::Fn; }

private:
    FnPtr m_fn{nullptr};
};

class ExceptionValue : public Value
{
public:
    ExceptionValue(std::string message)
        : m_message{message} {}
    virtual std::string inspect()
    {
        return "<exception" + m_message + ">";
    }
    virtual Type type() { return Type::Exception; }

    const std::string &message() { return m_message; }

private:
    std::string m_message;
};