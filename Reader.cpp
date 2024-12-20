// reader.cpp
#include "Reader.h"
#include <iostream>
#include "Types.h"
#include "Tokenizer.h"

// Constructor
Reader::Reader(std::vector<std::string> tokens) : tokens(std::move(tokens)), index(0) {}

// Member functions
std::optional<std::string> Reader::next()
{
    if (index < tokens.size())
    {
        return tokens.at(index++);
    }
    return std::nullopt;
}

std::optional<std::string> Reader::peek()
{
    if (index < tokens.size())
    {
        return tokens.at(index);
    }
    return std::nullopt;
}

// Static functions
std::vector<std::string> Reader::tokenize(std::string input)
{
    Tokenizer tokenizer = Tokenizer(input);
    std::vector<std::string> tokenizer_vector;

    while (auto token = tokenizer.next())
    {
        tokenizer_vector.push_back(*token);
    }
    return tokenizer_vector;
}

Value *Reader::read_str(std::string input)
{
    auto tokens = tokenize(input);
    Reader reader = Reader(tokens);
    return read_form(reader);
}

Value *Reader::read_form(Reader &reader)
{
    auto maybe_token = reader.peek();
    if (!maybe_token)
    {
        return nullptr;
    }
    auto token = maybe_token.value();
    switch (token[0])
    {
    case '(':
        return read_list(reader);
    case '[':
        return read_vector(reader);
    case '{':
        return read_hash_map(reader);
    case '\'':
        return read_quote(reader);
    case '`':
        return read_quote(reader);
    case '~':
        return read_quote(reader);

    case '@':
        return read_quote(reader);
    case '-':
        if (reader.peek().value().length() == 1)
            return read_atom(reader);
        return read_integer(reader);
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        return read_integer(reader);
    default:
        if(token == "T") {
            reader.next();
            return new TrueValue {};
        } else if (token == "false") {
            reader.next();
            return new FalseValue {};
        } else if (token == "nil") {
            reader.next();
            return new NillValue {};
        }
        return read_atom(reader);
    }
}

ListValue *Reader::
    read_list(Reader &reader)
{
    reader.next();
    auto *list = new ListValue();
    while (auto token = reader.peek())
    {
        if (*token == ")")
        {
            reader.next();
            return list;
        }
        list->push(read_form(reader));
    }
    std::cerr << "EOF\n";
    return list;
}

Value *Reader::read_integer(Reader &reader)
{
    auto token = reader.next();
    long num = 0;
    bool neg = false;
    for (char c : *token)
    {
        if (c == '-')
            neg = true;
        else
        {
            num *= 10;
            int digit = c - 48;
            num += digit;
        }
    }
    if (neg)
        num *= -1;
    return new IntegerValue{num};
}

VectorValue *Reader::read_vector(Reader &reader)
{
    reader.next();
    auto *list = new VectorValue();
    while (auto token = reader.peek())
    {
        if (*token == "]")
        {
            reader.next();
            return list;
        }
        list->push(read_form(reader));
    }
    std::cerr << "EOF\n";
    return list;
}
HashMapValue *Reader::read_hash_map(Reader &reader)
{
    reader.next();
    auto *map = new HashMapValue();
    while (auto token = reader.peek())
    {
        if (*token == "}")
        {
            reader.next();
            return map;
        }
        auto key = read_form(reader);
        token = reader.peek();
        if (*token == "}")
        {
            std::cerr << "Hash key without value\n";
            reader.next();
            return map;
        }
        auto val = read_form(reader);
        map->set(key, val);
    }
    std::cerr << "EOF\n";
    return map;
}
Value *Reader::read_quote(Reader &reader)
{
    auto token = reader.peek();
    auto *list = new ListValue();
    switch (token.value()[0])
    {
    case '\'':
        reader.next();
        list = new ListValue();
        list->push(new SymbolValue("quote"));
        list->push(read_form(reader));
        return list;
    case '`':
        reader.next();
        list = new ListValue();
        list->push(new SymbolValue("quasiquote"));
        list->push(read_form(reader));
        return list;
    case '~':
        if (token.value().length() > 1 && token.value()[1] == '@')
        {
            reader.next();
            list = new ListValue();
            list->push(new SymbolValue("splice-unquote"));
            list->push(read_form(reader));
            return list;
        }
        else
        {
            reader.next();
            list = new ListValue();
            list->push(new SymbolValue("unquote"));
            list->push(read_form(reader));
            return list;
        }
    case '@':
        reader.next();
        list = new ListValue();
        list->push(new SymbolValue("deref"));
        list->push(read_form(reader));
        return list;
    default:
        std::cerr << "Not standard quote!\n";
        return nullptr;
        abort();
    }
}

Value *Reader::read_atom(Reader &reader)
{
    return new SymbolValue(*reader.next());
}