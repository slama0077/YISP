#include <iostream>
#include <string>
#include "Reader.h"
#include "env.h"
#include "core.h"
#include "Printer.h"

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
        if(first->is_symbol()){
            auto special = first->as_symbol();
            if(special->matches("def!")) {
                auto key = list->at(1)->as_symbol();
                auto val = EVAL(list->at(2), env);
                env.set(key, val);
                return val;
            } else if(special->matches("let*")) {
                auto new_env = new Env {&env};
                auto bindings = list->at(1)->as_list();
                for(size_t i=0; i<bindings->size(); i+=2){
                    auto key = bindings->at(i)->as_symbol();
                    assert(i<bindings->size());
                    auto val = EVAL(bindings->at(i+1), *new_env);
                    new_env->set(key, val);
                }
                return EVAL(list->at(2), *new_env);
            } else if(special->matches("do")) {
                Value *result = nullptr;
                assert(list->size() > 1);
                for(size_t i=1; i<list->size(); i++){
                    result = EVAL(list->at(i), env);
                }
                return result;
            } else if(special->matches("if")) {
                auto condition = list->at(1);
                auto true_expr = list->at(2);
                auto false_expr = list->size() >= 4 ? list->at(3) : NillValue::the();
                if (EVAL(condition, env)->is_truthy()) return EVAL(true_expr, env);
                else return EVAL(false_expr, env);
            } else if(special->matches("fn*")) 
            {
                auto env_ptr = &env;
                auto binds = list->at(1)->as_list();
                auto body = list->at(2);
                auto closure = [env_ptr, binds, body](size_t argc, Value **args) {
                    auto exprs = new ListValue {};
                    for(size_t i=0; i<argc; i++){
                        exprs->push(args[i]);
                    }
                    auto fn_env = new Env { env_ptr, binds, exprs };
                    return EVAL(body, *fn_env);
                    // return NillValue::the() {};
                };
                return new FnValue {closure};
            }
        }
        auto eval_list = eval_ast(input, env)->as_list();
        auto fn = eval_list->at(0)->as_fn()->to_fn();
        Value *args[eval_list->size() - 1];
        for (size_t i = 1; i < eval_list->size(); i++)
        {
            args[i - 1] = eval_list->at(i);
        }
        return fn(eval_list->size() - 1, args); 
    }
}

std::string PRINT(Value *input)
{
    return pr_str(input, true);
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

int main()
{
    auto env = new Env {nullptr};
    auto ns = build_namespace();
    for(auto pair : ns) {
        env->set(new SymbolValue(pair.first), new FnValue(pair.second));
    }
    std::string input;

    while (1)
    {
        std::cout << "user (CTRL + C to exit)> ";
        std::getline(std::cin, input);
        if(input.empty()){
            std::cout << "Exiting..." << std::endl;
            return 0;
        }
        std::cout << rep(input, *env) << std::endl;
    }
    return 0;
}