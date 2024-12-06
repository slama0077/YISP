#include "Printer.h"

std::string pr_str(Value *value) {
        return value->inspect();
}