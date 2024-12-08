// types.h
#pragma once

#include <string>
#include <vector>
#include <cassert>
#include <unordered_map>
#include <functional>

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
class StringValue;
class KeywordValue;

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
        String,
        Keyword,
    };
    virtual ~Value() = default;
    virtual std::string inspect(bool print_readably = false) = 0; // Defined in types.cpp
    virtual bool is_symbol() {return false; };
    virtual bool is_list() {return false; };
    virtual bool is_listy() {return false;}
    virtual bool is_integer() {return false;}
    virtual bool is_exception() {return false;}
    virtual bool is_truthy() {return true; };
    virtual bool is_true() {return false;}
    virtual bool is_false() {return false;}
    virtual bool is_nil() {return false;}
    virtual bool is_string() {return false;}
    virtual bool is_keyword() {return false;}
    virtual bool operator==(Value *) { return false; };
    bool operator!=(Value *other) { return !(*this == other); };

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
    StringValue *as_string();
    KeywordValue* as_keyword();

    // const ListValue *as_list() const;
    // const VectorValue *as_vector() const;
    // const HashMapValue *as_hash_map() const;
    // const SymbolValue *as_symbol() const;
    // const IntegerValue *as_integer() const;
    // const FnValue *as_fn() const;
    // const ExceptionValue *as_exception() const;
    // const TrueValue *as_true() const;
    // const FalseValue *as_false() const;
    // const NillValue *as_nill() const;
    // const StringValue *as_string() const;
    //const KeywordValue* as_keyword() const;


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
    std::string inspect(bool print_readably = false) override;
    virtual Type type() { return Type::List; }
    virtual bool is_list() override {return true;};
    auto begin() { return listValue.begin(); }
    auto end() { return listValue.end(); }
    bool is_empty() { return listValue.size() == 0; }
    std::size_t size() { return listValue.size(); }
    Value *at(std::size_t index) { return listValue[index]; }

protected:
    std::vector<Value *> listValue;
};

class VectorValue : public ListValue
{

public:
    VectorValue();
    std::string inspect(bool print_readably = false) override;
    virtual bool is_listy() override {return true;}
    virtual Type type() { return Type::Vector; }
};

class HashMapValue : public Value
{
public:
    HashMapValue();
    std::string inspect(bool);
    virtual bool operator==(Value *) override;
    void set(Value *key, Value *val);
    Value *get(Value *key);
    virtual Type type() { return Type::HashMap; }
    auto begin() { return map.begin(); }
    auto end() { return map.end(); }
    std::size_t size() {return map.size();}
private:
    std::unordered_map<Value *, Value *, HashMapHash, HashMapPred> map;
};

class SymbolValue : public Value
{
public:
    explicit SymbolValue(std::string symbol);

    bool matches(char *nstr) {return symbol == nstr;}

    std::string str();
    std::string inspect(bool) override;
    bool operator==(Value *other) override {
        return other->is_symbol() && other->as_symbol()->symbol == symbol;
    }
    virtual Type type() { return Type::Symbol; }
    virtual bool is_symbol() override {return true;}
private:
    std::string symbol;
};

using Function = std::function<Value *(std::size_t, Value **)>;

class IntegerValue : public Value
{
public:
    IntegerValue(long l)
        : m_long{l} {}

    virtual std::string inspect(bool)
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
    virtual std::string inspect(bool)
    {
        return "<fn>";
    }
    bool operator==(Value *other) override {
        return other == this;
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
    virtual std::string inspect(bool)
    {
        return "<exception" + m_message + ">";
    }
    bool is_exception() override
    {
        return true;
    }

    bool operator==(Value *other) override {
        return other->is_exception() && other->as_exception()->m_message == m_message;
    }
    virtual Type type() { return Type::Exception; }

    const std::string &message() { return m_message; }

private:
    std::string m_message;
};

class TrueValue : public Value {
    public:
        static TrueValue* the()
        {
            if (!s_instance)
            {
                s_instance = new TrueValue;
            }
            return s_instance;
        }    
        virtual Type type() override { return Type::True; }
        virtual std::string inspect(bool) override { return "true"; }
        bool is_true() override
        {
            return true; 
        }
        bool operator==(Value *other) override {
            return other->is_true();
        }

    private:
    static inline TrueValue *s_instance { nullptr };
};

class FalseValue : public Value {
    public:
        static FalseValue* the()
        {
            if (!s_instance)
            {
                s_instance = new FalseValue;
            }
            return s_instance;
        }
        virtual Type type() override { return Type::False; }
        virtual std::string inspect(bool) override { return "false"; }
        bool is_false() override
        {
            return true; 
        }
        bool operator==(Value *other) override 
        {
            return other->is_false();
        }
        virtual bool is_truthy() override {return false; };
    
    private:
    static inline FalseValue *s_instance { nullptr };
};

class NillValue : public Value {
    public:
        static NillValue* the()
        {
            if (!s_instance)
            {
                s_instance = new NillValue;
            }
            return s_instance;
        }
        virtual Type type() override { return Type::Nill; }
        virtual std::string inspect(bool) override { return "nill"; }
        bool is_nil() override
        {
            return true; 
        }
        bool operator==(Value *other) override 
        {
            return other->is_nil();
        }
        virtual bool is_truthy() override {return false; };

    private:
    static inline NillValue *s_instance { nullptr };
};

class StringValue : public Value {
    public:
    StringValue(std::string str)
        : n_str {str} {}

    std::string str()
    {
        return n_str;
    }

    virtual Type type() override
    {
        return Type::String;
    }
    bool is_string() override
    {
        return true; 
    }

    bool operator==(Value *other) override 
    {
        return other->is_string() && other->as_string()->n_str == n_str;
    }

    std::string inspect(bool print_readably = false) override 
    { 
        if(print_readably)
        {
            std::string str = "\"";
            for (char c : n_str)
            {
                switch (c) 
                {
                    case '"':  // Handle the case where c is a double-quote
                        str += '\\';
                        str += c;
                        break;

                    case '\\':  // Handle the case where c is a backslash
                        str += '\\';
                        str += '\\';
                        break;
                    case '\n':  // Handle the case where c is a backslash
                        str += '\\';
                        str += 'n';
                        break;
                    default:  // Handle all other cases
                        str += c;
                        break;
                }

            }
            return str + "\""; 
        }
       return n_str;
    }


    private:
    std::string n_str;

};


class KeywordValue: public Value
{
    public:
    KeywordValue(std::string str)
        : n_str {str} {}

    std::string str()
    {
        return n_str;
    }
    virtual Type type() override
    {
        return Type::Keyword;
    }
    bool is_keyword() override
    {
        return true; 
    }
    bool operator==(Value *other) override 
    {
        return other->is_keyword() && other->as_keyword()->n_str == n_str;
    }
    std::string inspect(bool) override { return  n_str; }
    private:
    std::string n_str;
};
