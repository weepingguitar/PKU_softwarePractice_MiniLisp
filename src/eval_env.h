#ifndef EVAL_ENV_H
#define EVAL_ENV_H

#include <unordered_map>
#include "./value.h"

class EvalEnv{
    std::unordered_map<std::string, ValuePtr> symbolTable;
public:
    EvalEnv();
    std::vector<ValuePtr> evalList(ValuePtr expr);
    ValuePtr eval(ValuePtr expr);
    ValuePtr apply(ValuePtr proc, std::vector<ValuePtr>& args);
};

#endif