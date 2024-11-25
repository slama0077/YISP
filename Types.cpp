// types.cpp
#include "Types.h"

// Value class implementation
std::string Value::inspect() {
    assert(0);  // This is a pure virtual-like behavior
    return "";
}

// ListValue class implementation
ListValue::ListValue() {}

void ListValue::push(Value* value) {
    listValue.push_back(value);
}

std::string ListValue::inspect() {
    std::string out = "(";
    for (auto* value : listValue) {
        out.append(value->inspect());
        out.append(" ");
    }
    if (!listValue.empty()) {
        out[out.length() - 1] = ')';
    } 
    else {
        out += ")";
    }
    return out;
}

// SymbolValue class implementation
SymbolValue::SymbolValue(std::string symbol) : symbol(std::move(symbol)) {}

std::string SymbolValue::str() {
    return symbol;
}

std::string SymbolValue::inspect() {
    return str();
}
