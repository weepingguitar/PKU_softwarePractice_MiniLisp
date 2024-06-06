#ifndef EVAL_ENV_H
#define EVAL_ENV_H

#include <unordered_map>
#include "./value.h"

class EvalEnv : public std::enable_shared_from_this<EvalEnv>{
    std::unordered_map<std::string, ValuePtr> symbolTable;
    std::shared_ptr<EvalEnv> parent = nullptr;
public:
    EvalEnv();
    static std::shared_ptr<EvalEnv> createGlobal();
    std::shared_ptr<EvalEnv> createChild(const std::vector<std::string>& params, const std::vector<ValuePtr>& args);
    void addparent(std::shared_ptr<EvalEnv> parent);
    std::vector<ValuePtr> evalList(ValuePtr expr);
    ValuePtr eval(ValuePtr expr);
    ValuePtr apply(ValuePtr proc, std::vector<ValuePtr>& args);
    ValuePtr addValue(std::string name, ValuePtr val);
    ValuePtr lookupBinding(const std::string& name);
};

#endif