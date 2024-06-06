#include <algorithm>
#include <iterator>

#include "builtins.h"
#include "error.h"
#include "value.h"
#include "eval_env.h"
#include "forms.h"

using namespace std::literals;

EvalEnv::EvalEnv(){
    for(auto item : BuiltinFuncs){
        symbolTable.insert(item);
    }
}

std::shared_ptr<EvalEnv> EvalEnv::createGlobal(){
    return std::make_shared<EvalEnv>();
}

std::shared_ptr<EvalEnv> EvalEnv::createChild(const std::vector<std::string>& params, const std::vector<ValuePtr>& args){
    auto child = std::make_shared<EvalEnv>();
    for (int i = 0; i < params.size();i++){
        child->addValue(params[i], args[i]);
    }
    child->addparent((*this).shared_from_this());
    return child;
}

void EvalEnv::addparent(std::shared_ptr<EvalEnv> parent){
    this->parent = parent;
}

std::vector<ValuePtr> EvalEnv::evalList(ValuePtr expr){
    std::vector<ValuePtr> ret{};
    if(typeid(*expr) == typeid(NilValue)){
        return ret;
    }

    std::ranges::transform(expr->toVector(), 
                           std::back_inserter(ret),
                           [this](ValuePtr V) { return this->eval(V); });

    return ret;
}

ValuePtr EvalEnv::apply(ValuePtr proc, std::vector<ValuePtr>& args){
    if(typeid(*proc) == typeid(BuiltinProcValue)){
        return static_cast<BuiltinProcValue&>(*proc).call(args, (*this));
    }
    else if(typeid(*proc) == typeid(LambdaValue)){
        return static_cast<LambdaValue&>(*proc).apply(args);
    }
    else{
        throw LispError("Unimplemented");
    }
}

ValuePtr EvalEnv::eval(ValuePtr expr){
    if(typeid(*expr) == typeid(BooleanValue)
      ||typeid(*expr) == typeid(NumericValue)
      ||typeid(*expr) == typeid(StringValue))
    {
        return expr;
    }
    else if(typeid(*expr) == typeid(NilValue))
    {
        throw LispError("Evaluating nil is prohibited.");
    }
    else if(typeid(*expr) == typeid(PairValue))
    {
        ValuePtr leftval = static_cast<PairValue&>(*expr).getLeft();
        ValuePtr rightval = static_cast<PairValue&>(*expr).getRight();
        
        if(auto name = leftval->asSymbol()){
            if(SPECIAL_FORMS.contains(*name))
            {
                return SPECIAL_FORMS[(*name)](rightval, *this);
            }
        }

        ValuePtr proc = this->eval(leftval);
        std::vector<ValuePtr> args = this->evalList(rightval);

        return this->apply(proc, args);
    }
    else if(auto name = expr->asSymbol())
    {
        return this->lookupBinding(*name);
    }

    else if(typeid(*expr) == typeid(BuiltinProcValue)){
        return expr;
    }

    else{
        throw LispError("Unimplemented");
    }
    return std::make_shared<NilValue>();
}

ValuePtr EvalEnv::addValue(std::string name, ValuePtr val){
    if(symbolTable.contains(name)){
        symbolTable[name] = val;
    }
    else{
        symbolTable.insert({name, val});
    }
    return std::make_shared<NilValue>();
}

ValuePtr EvalEnv::lookupBinding(const std::string& name){
    if(auto value = this->symbolTable.contains(name)){
        return symbolTable[name];
    }
    if(this->parent == nullptr){
        throw LispError("Variable not defined.");
    }
    return this->parent->lookupBinding(name);

}