#include <iostream>

#include "core.h"
#include "Printer.h"

std::unordered_map<std::string, Function> build_namespace() {
    std::unordered_map<std::string, Function> ns;
    ns["+"] = add;
    ns["-"] = subtract;
    ns["*"] = multiply;
    ns["/"] = divide;
    ns["eval"] = prn;
    ns["quote"] = list;
    ns["list?"] = list_q;
    ns["empty?"] = empty_q;
    ns["count"] = count;
    ns["="] = eq;
    ns["<"] = lt;
    ns["<="] = lte;
    ns[">"] = gt;
    ns[">="] = gte;
    return ns;

}

Value *add(size_t argc, Value **args)
{
    assert(argc == 2);
    auto a = args[0];
    auto b = args[1];

    assert(a->type() == Value::Type::Integer);
    assert(b->type() == Value::Type::Integer);

    long result = a->as_integer()->to_long() + b->as_integer()->to_long();
    return new IntegerValue{result};
}
Value *subtract(size_t argc, Value **args)
{
    assert(argc == 2);
    auto a = args[0];
    auto b = args[1];

    assert(a->type() == Value::Type::Integer);
    assert(b->type() == Value::Type::Integer);

    long result = a->as_integer()->to_long() - b->as_integer()->to_long();
    return new IntegerValue{result};
}

Value *divide(size_t argc, Value **args)
{
    assert(argc == 2);
    auto a = args[0];
    auto b = args[1];

    assert(a->type() == Value::Type::Integer);
    assert(b->type() == Value::Type::Integer);

    long result = a->as_integer()->to_long() / b->as_integer()->to_long();
    return new IntegerValue{result};
}
Value *multiply(size_t argc, Value **args)
{
    assert(argc == 2);
    auto a = args[0];
    auto b = args[1];

    assert(a->type() == Value::Type::Integer);
    assert(b->type() == Value::Type::Integer);

    long result = a->as_integer()->to_long() * b->as_integer()->to_long();
    return new IntegerValue{result};
}

Value *prn(size_t argc, Value **args){
    assert(argc >= 1);
    std::cout << pr_str(args[0]) << "\n";
    return new NillValue;
}
Value *list(size_t argc, Value **args){
    auto l = new ListValue {};

    for(size_t i=0; i<argc; i++){
        l->push(args[i]);
    }
    return l;
}
Value *list_q(size_t argc, Value **args){
    assert(argc >= 1);
    if(args[0]->is_list()) {
        return new TrueValue;
    } else {
        return new FalseValue;
    }
}
Value *empty_q(size_t argc, Value **args){
    assert(argc >= 1);
    if(args[0]->is_list() && args[0]->as_list()->is_empty()) {
        return new TrueValue;
    } else {
        return new FalseValue;
    }
 
}
Value *count(size_t argc, Value **args){
    assert(argc >= 1);
    if(args[0]->is_list()){
        return new IntegerValue {static_cast<long>(args[0]->as_list()->size()) };
    }
    return new IntegerValue { 0 };
}
Value *eq(size_t argc, Value **args){
    assert(argc >= 1);
    auto a = args[0];
    auto b = args[1];
    if(*a == b){
        return new TrueValue;
    }
    return new ListValue;

}
Value *lt(size_t argc, Value **args){
    assert(argc >= 2);
    auto a = args[0];
    auto b = args[1];
    assert(a->is_integer());
    assert(b->is_integer());
    if(a->as_integer()->to_long() < b->as_integer()->to_long()){
        return new TrueValue;
    }
    return new ListValue;
}
Value *lte(size_t argc, Value **args){
    assert(argc >= 2);
    auto a = args[0];
    auto b = args[1];
    assert(a->is_integer());
    assert(b->is_integer());
    if(a->as_integer()->to_long() <= b->as_integer()->to_long()){
        return new TrueValue;
    }
    return new ListValue;
}
Value *gt(size_t argc, Value **args){
    assert(argc >= 2);
    auto a = args[0];
    auto b = args[1];
    assert(a->is_integer());
    assert(b->is_integer());
    if(a->as_integer()->to_long() > b->as_integer()->to_long()){
        return new TrueValue;
    }
    return new ListValue;
}
Value *gte(size_t argc, Value **args){
    assert(argc >= 2);
    auto a = args[0];
    auto b = args[1];
    assert(a->is_integer());
    assert(b->is_integer());
    if(a->as_integer()->to_long() >= b->as_integer()->to_long()){
        return new TrueValue;
    }
    return new ListValue;
}
