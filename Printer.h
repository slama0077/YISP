#include <iostream>
#include <string>
#include "Types.h"

std::string pr_str(Value* value)
{
    return value->inspect();
}