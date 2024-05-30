#include <algorithm>
#include <iterator>

#include "builtins.h"
#include "error.h"
#include "value.h"
#include "eval_env.h"

using namespace std::literals;

EvalEnv::EvalEnv(){
    symbolTable.insert({"+", std::make_shared<BuiltinProcValue>(&add)});
    symbolTable.insert({"print", std::make_shared<BuiltinProcValue>(&print)});
}

std::vector<ValuePtr> EvalEnv::evalList(ValuePtr expr){
    std::vector<ValuePtr> ret;
    std::ranges::transform(expr->toVector(), 
                           std::back_inserter(ret),
                           [this](ValuePtr V) { return this->eval(V); });
    return ret;
}

ValuePtr EvalEnv::apply(ValuePtr proc, std::vector<ValuePtr>& args){
    if(typeid(*proc) == typeid(BuiltinProcValue)){
        return static_cast<BuiltinProcValue&>(*proc).call(args);
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
        auto values = (*expr).toVector();
        if(values[0]->asSymbol() == "define"s){
            if(auto name = values[1]->asSymbol()){
                ValuePtr val = this->eval(values[2]);
                if (symbolTable.contains(*name)) 
                    symbolTable[*name] = val;
                else   
                    symbolTable.insert({(*name), val});
                ValuePtr v{new NilValue};
                return v;
            }
            else{
                throw LispError("Malformed define.");
            }
        }
        else{
            ValuePtr proc = this->eval(values[0]);
            std::vector<ValuePtr> args;
            ValuePtr left = static_cast<PairValue&>(*expr).getRight();
            if(typeid(*left) == typeid(NilValue))
            {
                args = {};
            }
            else{
                args = this->evalList(left);
            }
            return this->apply(proc, args);
        }
    }
    else if(auto name = expr->asSymbol())
    {
        if (auto value = symbolTable.contains(*name)) 
            return symbolTable[*name];
        else{
            throw LispError("Variable " + *name + " not defined.");
        }
    }

    else if(typeid(*expr) == typeid(BuiltinProcValue)){
        return expr;
    }

    else{
        throw LispError("Unimplemented");
    }
}