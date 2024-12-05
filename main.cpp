#include <iostream>
#include <string>
#include "Reader.h"
#include "Printer.h"
#include "env.h"

Value *READ(std::string input)
{
    return Reader::read_str(input);
    // return input;
    // Tokenizer tokenizer = Tokenizer(input);
    // while(true)
    // {
    //     auto token = tokenizer.next();
    //     if (!token) break;  // Exit the loop if `next()` returns an empty optional
    //     std::cout << "[" << *token << "]\n";  // Dereference to access the value
    // }
    // return input;
}
Value *EVAL(Value *input, Env &env);

Value *eval_ast(Value *ast, Env &env)
{
    switch (ast->type())
    {
    case Value::Type::Symbol:
    {
        return env.get(ast->as_symbol());
    }
    case Value::Type::List:
    {
        auto result = new ListValue{};
        for (auto val : *ast->as_list())
        {
            result->push(EVAL(val, env));
        }
        return result;
    }
    case Value::Type::Vector:
    {
        auto result = new VectorValue{};
        for (auto val : *ast->as_vector())
        {
            result->push(EVAL(val, env));
        }
        return result;
    }
    case Value::Type::HashMap:
    {
        auto result = new HashMapValue{};
        for (auto pair : *ast->as_hash_map())
        {
            result->set(pair.first, EVAL(pair.second, env));
        }
        return result;
    }
    default:
        return ast;
    }
}

Value *EVAL(Value *input, Env &env)
{
    if (input->type() != Value::Type::List)
    {
        return eval_ast(input, env);
    }
    else if (input->as_list()->is_empty())
    {
        return input;
    }
    else
    {
        auto list = input->as_list();
        auto first = list->at(0);
        if(first->is_symbol() && first->as_symbol()->matches("def!")) {
            auto key = list->at(1)->as_symbol();
            auto val = EVAL(list->at(2), env);
            env.set(key, val);
            return val;
        } else if(first->is_symbol() && first->as_symbol()->matches("let*")) {
            auto new_env = new Env {&env};
            auto bindings = list->at(1)->as_list();
            for(size_t i=0; i<bindings->size(); i+=2){
                auto key = bindings->at(i)->as_symbol();
                assert(i<bindings->size());
                auto val = EVAL(bindings->at(i+1), *new_env);
                new_env->set(key, val);
            }
            return EVAL(list->at(2), *new_env);
        } else {
            auto list = eval_ast(input, env)->as_list();
            auto fn = list->at(0)->as_fn()->to_fn();
            Value *args[list->size() - 1];
            for (size_t i = 1; i < list->size(); i++)
            {
                args[i - 1] = list->at(i);
            }
            return fn(list->size() - 1, args);
        }
    }
}

std::string PRINT(Value *input)
{
    return pr_str(input);
}

std::string rep(std::string input, Env &env)
{
    try
    {
        auto ast = READ(input);
        auto result = EVAL(ast, env);
        return PRINT(result);
    }
    catch (ExceptionValue *exception)
    {
        std::cerr << exception->message() << std::endl;
        return "";
    }
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
Value *sub(size_t argc, Value **args)
{
    assert(argc == 2);
    auto a = args[0];
    auto b = args[1];

    assert(a->type() == Value::Type::Integer);
    assert(b->type() == Value::Type::Integer);

    long result = a->as_integer()->to_long() - b->as_integer()->to_long();
    return new IntegerValue{result};
}

Value *div(size_t argc, Value **args)
{
    assert(argc == 2);
    auto a = args[0];
    auto b = args[1];

    assert(a->type() == Value::Type::Integer);
    assert(b->type() == Value::Type::Integer);

    long result = a->as_integer()->to_long() / b->as_integer()->to_long();
    return new IntegerValue{result};
}
Value *mul(size_t argc, Value **args)
{
    assert(argc == 2);
    auto a = args[0];
    auto b = args[1];

    assert(a->type() == Value::Type::Integer);
    assert(b->type() == Value::Type::Integer);

    long result = a->as_integer()->to_long() * b->as_integer()->to_long();
    return new IntegerValue{result};
}
int main()
{
    auto env = new Env {nullptr};

    env->set(new SymbolValue("+"), new FnValue{add});
    env->set(new SymbolValue("-"), new FnValue{sub});
    env->set(new SymbolValue("*"), new FnValue{mul});
    env->set(new SymbolValue("/"), new FnValue{div});
    std::string input;

    while (1)
    {
        std::cout << "user (CTRL + C to exit)> ";
        std::getline(std::cin, input);
        std::cout << rep(input, *env) << std::endl;
    }
    return 0;
}