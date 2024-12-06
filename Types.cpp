// types.cpp
#include "Types.h"

ListValue *Value::as_list()
{
    assert(type() == Type::List || type() == Type::Vector);
    return static_cast<ListValue *>(this);
}
bool ListValue::operator==(Value *other) {
    if(type() != other->type()){
        return false;
    }
    if(size() != other->as_list()->size()){
        return false;
    }
    auto other_list = other->as_list();
    for(size_t i=0; i<size(); i++){
        if(!(*at(i) == other_list->at(i))) {
            return false;
        }
    }
    return true;
}
VectorValue *Value::as_vector()
{
    assert(type() == Type::Vector);
    return static_cast<VectorValue *>(this);
}
HashMapValue *Value::as_hash_map()
{
    assert(type() == Type::HashMap);
    return static_cast<HashMapValue *>(this);
}
SymbolValue *Value::as_symbol()
{
    assert(type() == Type::Symbol);
    return static_cast<SymbolValue *>(this);
}
IntegerValue *Value::as_integer()
{
    assert(type() == Type::Integer);
    return static_cast<IntegerValue *>(this);
}
FnValue *Value::as_fn()
{
    assert(type() == Type::Fn);
    return static_cast<FnValue *>(this);
}
ExceptionValue *Value::as_exception()
{
    assert(type() == Type::Exception);
    return static_cast<ExceptionValue *>(this);
}

TrueValue *Value::as_true()
{
    assert(type() == Type::True);
    return static_cast<TrueValue *>(this);
}
FalseValue *Value::as_false()
{
    assert(type() == Type::False);
    return static_cast<FalseValue *>(this);
}

NillValue *Value::as_nill()
{
    assert(type() == Type::Nill);
    return static_cast<NillValue *>(this);
}


// Value class implementation
std::string Value::inspect()
{
    assert(0); // This is a pure virtual-like behavior
    return "";
}

// ListValue class implementation
ListValue::ListValue() {}

void ListValue::push(Value *value)
{
    listValue.push_back(value);
}

std::string ListValue::inspect()
{
    std::string out = "(";
    for (auto *value : listValue)
    {
        out.append(value->inspect());
        out.append(" ");
    }
    if (!listValue.empty())
    {
        out[out.length() - 1] = ')';
    }
    else
    {
        out += ")";
    }
    return out;
}

VectorValue::VectorValue() {}

std::string VectorValue::inspect()
{
    std::string out = "[";
    for (auto *value : listValue)
    {
        out.append(value->inspect());
        out.append(" ");
    }
    if (!listValue.empty())
    {
        out[out.length() - 1] = ']';
    }
    else
    {
        out += "]";
    }
    return out;
}

HashMapValue::HashMapValue() {}

std::string HashMapValue::inspect()
{
    std::string out = "{ ";
    for (auto value : map)
    {
        out.append(value.first->inspect());
        out.append(" ");
        out.append(value.second->inspect());
        // out.append(" ");
    }
    if (map.empty())
    {
        out[out.length() - 1] = '}';
    }
    else
    {
        out += "}";
    }
    return out;
}

void HashMapValue::set(Value *key, Value *val)
{
    map[key] = val;
}

Value *HashMapValue::get(Value *key)
{
    auto search = map.find(key);
    if (search != map.end())
    {
        return search->second;
    }
    return nullptr;
}
// SymbolValue class implementation
SymbolValue::SymbolValue(std::string symbol) : symbol(std::move(symbol)) {}

std::string SymbolValue::str()
{
    return symbol;
}

std::string SymbolValue::inspect()
{
    return str();
}
