#ifndef BUILTINS_H
#define BUILTINS_H

#include <unordered_map>

#include "./value.h"
#include "./error.h"
#include "eval_env.h"

extern const std::unordered_map<std::string, ValuePtr> BuiltinFuncs;

#endif