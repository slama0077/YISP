// types.h
#pragma once

#include <string>
#include <vector>
#include <cassert>
#include <unordered_map>
#include <functional>
#include <iostream>

class ListValue;
class VectorValue;
class HashMapValue;
class SymbolValue;
class IntegerValue;
class FnValue;
class ExceptionValue;
class TrueValue;
class FalseValue;
class NillValue;
class ConsValue;

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
        True,
        False,
        Nill,
        Cons,
    };
    virtual ~Value() = default;
    virtual std::string inspect() = 0; // Defined in types.cpp
    virtual bool is_symbol() {return false; };
    virtual bool is_truthy() {return true; };
    virtual bool is_list() {return false; };
    virtual bool is_cons() {return false;};
    virtual bool is_listy() {return false;}
    virtual bool is_integer() {return false;}
    virtual bool is_nil() {return false;}
    virtual bool operator==(Value *) { return false; };
    virtual Type type() = 0;
    ListValue *as_list();
    VectorValue *as_vector();
    HashMapValue *as_hash_map();
    SymbolValue *as_symbol();
    IntegerValue *as_integer();
    FnValue *as_fn();
    ExceptionValue *as_exception();
    TrueValue *as_true();
    FalseValue *as_false();
    NillValue *as_nill();
    ConsValue *as_cons();
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
    virtual bool operator==(Value *) override;
    std::string inspect() override;
    virtual Type type() { return Type::List; }
    virtual bool is_list() override {return true;};
    auto begin() { return listValue.begin(); }
    auto end() { return listValue.end(); }
    bool is_empty() { return listValue.size() == 0; }
    size_t size() { return listValue.size(); }
    virtual bool is_truthy() override {return listValue.size(); }
    virtual bool is_nil() override {
        return listValue.size() == 0;};
    Value *at(size_t index) { return listValue[index]; }

protected:
    std::vector<Value *> listValue;
};

class ConsValue : public Value
{
    public:
        ConsValue(Value* car, Value* cdr) : m_car(car), m_cdr(cdr) {};
        std::string inspect() {return "(" + m_car->inspect() + " . " + m_cdr->inspect() + ")"; };
        virtual Type type() { return Type::Cons; }
        virtual bool is_cons() override {return true; }

        Value* m_car;
        Value* m_cdr;
};

class VectorValue : public ListValue
{

public:
    VectorValue();
    std::string inspect() override;
    virtual bool is_listy() override {return true;}
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
    bool matches(char *nstr) {
        std::transform(symbol.begin(), symbol.end(), symbol.begin(), ::tolower);
        return symbol == nstr;
        }

    std::string str();
    std::string inspect() override;
    virtual Type type() { return Type::Symbol; }
    virtual bool is_symbol() override {return true;}
private:
    std::string symbol;
};

using Function = std::function<Value *(size_t, Value **)>;

class IntegerValue : public Value
{
public:
    IntegerValue(long l)
        : m_long{l} {}

    virtual std::string inspect()
    {
        return std::to_string(m_long);
    }
    virtual bool is_integer() {return true;}
    bool operator==(Value *other) override {
        return other->is_integer() && other->as_integer()->m_long == m_long;
    }
    long to_long() { return m_long; }
    virtual Type type() { return Type::Integer; }

private:
    long m_long{0};
};

class FnValue : public Value
{
public:
    FnValue(Function fn)
        : m_fn{fn} {}
    virtual std::string inspect()
    {
        return "<fn>";
    }
    Function to_fn() { return m_fn; }
    virtual Type type() { return Type::Fn; }

private:
    Function m_fn{nullptr};
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

class TrueValue : public Value {
    public:
        virtual Type type() override { return Type::True; }
        virtual std::string inspect() override { return "T"; }
        bool operator==(Value *other) override {
        return other->inspect() == "T";
    }
};

class FalseValue : public Value {
    public:
        virtual Type type() override { return Type::False; }
        virtual std::string inspect() override { return "false"; }
        virtual bool is_truthy() override {return false; };
        virtual bool is_nil() override {return true;};
        bool operator==(Value *other) override {
        return other->inspect() == "false";
    }
};

class NillValue : public Value {
    public:
        virtual Type type() override { return Type::Nill; }
        virtual std::string inspect() override { return "nil"; }
        virtual bool is_truthy() override {return false; };
        virtual bool is_nil() override {return true;};
        bool operator==(Value *other) override {
        return other->inspect() == "nil";
    }

};