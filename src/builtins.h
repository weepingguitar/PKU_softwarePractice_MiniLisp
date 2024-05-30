#ifndef BUILTINS_H
#define BUILTINS_H

#include "./value.h"
#include "./error.h"

ValuePtr add(const std::vector<ValuePtr>& params);

ValuePtr print(const std::vector<ValuePtr>& params);

#endif