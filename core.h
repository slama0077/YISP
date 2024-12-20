#pragma once

#include <unordered_map>
#include <string>
#include "types.h"


std::unordered_map<std::string, Function> build_namespace();

Value *add(size_t argc, Value **args);
Value *subtract(size_t argc, Value **args);
Value *multiply(size_t argc, Value **args);
Value *divide(size_t argc, Value **args);
Value *prn(size_t argc, Value **args);
Value *list(size_t argc, Value **args);
Value *list_q(size_t argc, Value **args);
Value *empty_q(size_t argc, Value **args);
Value *count(size_t argc, Value **args);
Value *eq(size_t argc, Value **args);
Value *lt(size_t argc, Value **args);
Value *lte(size_t argc, Value **args);
Value *gt(size_t argc, Value **args);
Value *gte(size_t argc, Value **args);
