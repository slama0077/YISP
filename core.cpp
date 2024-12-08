#include <iostream>

#include "core.h"
#include "Printer.h"

std::unordered_map<std::string, Function> build_namespace() {
    std::unordered_map<std::string, Function> ns;
    ns["+"] = add;
    ns["-"] = subtract;
    ns["*"] = multiply;
    ns["/"] = divide;
    ns["prn"] = prn;
    ns["list"] = list;
    ns["list?"] = list_q;
    ns["empty?"] = empty_q;
    ns["count"] = count;
    ns["="] = eq;
    ns["<"] = lt;
    ns["<="] = lte;
    ns[">"] = gt;
    ns[">="] = gte;
    ns["not"] = not_funcn;
    ns["pr-str"] = pr_str_funcn;
    ns["str"] = str;
    ns["println"] = println;
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

Value *prn(size_t argc, Value **args)
{
    if (argc == 0)
    {
        std::cout << "\n";
        return NillValue::the();
    }
    else {
        assert(argc >= 1);
        for (size_t i = 0; i < argc; i++)
       {
        std::cout << pr_str(args[i], true);
        if (i < argc - 1)
            {
                std::cout << ' ';
            }
       }
        std::cout << std::endl;
    }
    return NillValue::the();
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
        return TrueValue::the();
    } else {
        return FalseValue::the();
    }
}
Value *empty_q(size_t argc, Value **args){
    assert(argc >= 1);
    if(args[0]->is_listy() && args[0]->as_list()->is_empty()) {
        return TrueValue::the();
    } else {
        return FalseValue::the();
    }
 
}
Value *count(size_t argc, Value **args){
    assert(argc >= 1);
    if(args[0]->is_listy()){
        return new IntegerValue {static_cast<long>(args[0]->as_list()->size()) };
    }
    return new IntegerValue { 0 };
}
Value *eq(size_t argc, Value **args){
    assert(argc >= 1);
    auto a = args[0];
    auto b = args[1];
    if(*a == b){
        return TrueValue::the();
    }
    return FalseValue::the();

}
Value *lt(size_t argc, Value **args){
    assert(argc >= 2);
    auto a = args[0];
    auto b = args[1];
    assert(a->is_integer());
    assert(b->is_integer());
    if(a->as_integer()->to_long() < b->as_integer()->to_long()){
        return TrueValue::the();
    }
    return FalseValue::the();
}
Value *lte(size_t argc, Value **args){
    assert(argc >= 2);
    auto a = args[0];
    auto b = args[1];
    assert(a->is_integer());
    assert(b->is_integer());
    if(a->as_integer()->to_long() <= b->as_integer()->to_long()){
        return TrueValue::the();
    }
    return FalseValue::the();
}
Value *gt(size_t argc, Value **args){
    assert(argc >= 2);
    auto a = args[0];
    auto b = args[1];
    assert(a->is_integer());
    assert(b->is_integer());
    if(a->as_integer()->to_long() > b->as_integer()->to_long()){
        return TrueValue::the();
    }
    return FalseValue::the();
}
Value *gte(size_t argc, Value **args){
    assert(argc >= 2);
    auto a = args[0];
    auto b = args[1];
    assert(a->is_integer());
    assert(b->is_integer());
    if(a->as_integer()->to_long() >= b->as_integer()->to_long()){
        return TrueValue::the();
    }
    return FalseValue::the();
}

Value *not_funcn(size_t argc, Value **args)
{
    assert(argc >= 2);
    if(args[0]->is_truthy())
        return FalseValue::the();
    return TrueValue::the();
}

Value *pr_str_funcn(size_t argc, Value **args)
{
    std::string str = "";
    if (argc == 0)
    {
        return new StringValue { str };
    }
    else 
    {        
       for (size_t i = 0; i < argc; i++)
       {
            str += pr_str(args[i], true);
            if (i < argc - 1)
            {
                str += ' ';
            }
       }
    }
    return new StringValue { str };
}

Value *str(size_t argc, Value **args)
{
    std::string str = "";
    if (argc == 0)
    {
        std::cout << "\n";
        return NillValue::the();
    }
    else 
    {        
       for (size_t i = 0; i < argc; i++)
       {
        str += pr_str(args[i], false);
       }
    }
    return new StringValue { str };
}

Value *println(size_t argc, Value **args)
{
    std::string str = "";
    if (argc == 0)
    {
        return new StringValue { str };
    }
    else 
    {        
       for (size_t i = 0; i < argc; i++)
       {
            str += pr_str(args[i], false);
            if (i < argc - 1)
            {
                str += ' ';
            }
       }
       std::cout << str << "\n";
    }
    return NillValue::the();
}
