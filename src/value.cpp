#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>

#include "./value.h"
#include "error.h"
#include "eval_env.h"


Value::Value(){}
bool Value::isNumber(){
    if (typeid(*this) == typeid(NumericValue)) 
        return true;
    return false;
}

double Value::asNumber(){
    return static_cast<NumericValue&>(*this).getval();
}

std::vector<ValuePtr> Value::toVector() {
    throw LispError("Can't convert to vector!");
}

std::optional<std::string> Value::asSymbol(){
    return {};
}

BooleanValue::BooleanValue(bool val) : Value(), val{val} {}

std::string BooleanValue::toString() const {
    if (val)
        return "#t";
    else
        return "#f";
}

std::vector<ValuePtr> BooleanValue::toVector(){
    std::vector<ValuePtr> v{std::make_shared<BooleanValue>(this->val)};
    return v;
}

NumericValue::NumericValue(double val) : Value(), val{val} {}

double NumericValue::getval(){
    return val;
}

std::string NumericValue::toString() const {
    bool isint = 1;
    std::string ret = std::to_string(val);
    int decimalpos = ret.find('.');
    for (auto i = decimalpos + 1;i < ret.length(); i++)
    {
        if(ret[i] != '0')
        {
            isint = 0;
            break;
        }
    }
    if(isint)
        ret = ret.erase(decimalpos, ret.length() - decimalpos);
    return ret;

}

std::vector<ValuePtr> NumericValue::toVector(){
    std::vector<ValuePtr> v{std::make_shared<NumericValue>(this->val)};
    return v;
}

StringValue::StringValue(const std::string& val) : Value(), val{val} {}

std::string StringValue::toString() const {
    std::stringstream ss;
    ss << std::quoted(val);
    return ss.str();
}

std::string StringValue::getString(){
    return this->val;
}

std::vector<ValuePtr> StringValue::toVector(){
    std::vector<ValuePtr> v{std::make_shared<StringValue>(this->val)};
    return v;
}

NilValue::NilValue() : Value() {}

std::string NilValue::toString() const {
    return "()";
}

std::vector<ValuePtr> NilValue::toVector(){
    return std::vector<ValuePtr>();
}

SymbolValue::SymbolValue(const std::string& val) : Value(), val{val} {}

std::string SymbolValue::toString() const {
    return val;
}

std::optional<std::string> SymbolValue::asSymbol(){
    return this->val;
}

std::vector<ValuePtr> SymbolValue::toVector(){
    std::vector<ValuePtr> v{std::make_shared<SymbolValue>(this->val)};
    return v;
}

PairValue::PairValue(ValuePtr leftval, ValuePtr rightval) : Value(), leftval{leftval}, rightval{rightval} {}

PairValue::PairValue(const std::vector<ValuePtr>& params) : Value(), leftval{params[0]} {
    if(params.size() == 1){
        rightval = std::make_shared<NilValue>();
    }
    else{
        auto start = params.begin() + 1;
        auto end = params.end();
        std::vector<ValuePtr> temp(start, end);
        rightval = std::make_shared<PairValue>(temp);
    }
}

std::string PairValue::pair_toString(bool start) const {
    if(typeid(*rightval) != typeid(PairValue))
    {
        std::string firstword;
        if (start) 
            firstword = "(";
        else
            firstword = " ";
        if(typeid(*rightval) == typeid(NilValue))
        {
            return firstword + (*leftval).toString() + ")";
        }
        else{
            return firstword + (*leftval).toString() + " . " + (*rightval).toString() + ")";
        }
    }
    auto& pair = static_cast<const PairValue&>(*rightval);
    if(start)
    {
        return "(" + (*leftval).toString() + pair.pair_toString(0);
    }
    else{
        return " " + (*leftval).toString() + pair.pair_toString(0);
    }
}

std::string PairValue::toString() const {
    return pair_toString(1);
}

void PairValue::pushToVector(std::vector<ValuePtr>& ret){
    if(typeid(*leftval) != typeid(NilValue))
        ret.push_back(leftval);
    else{
        throw LispError("Can't evaluate nil value");
    }
    if(typeid(*rightval) != typeid(PairValue))
    {
        if(typeid(*rightval) != typeid(NilValue))
            ret.push_back(rightval);
        return;
    }
    static_cast<PairValue&>(*rightval).pushToVector(ret);
    return;
}

std::vector<ValuePtr> PairValue::toVector(){
    std::vector<ValuePtr> ret;
    if(typeid(*leftval) != typeid(NilValue))
        ret.push_back(this->leftval);
    else{
        throw LispError("Can't evaluate nil value");
    }
    if(typeid(*rightval) != typeid(PairValue))
    {
        if(typeid(*rightval) != typeid(NilValue))
            ret.push_back(rightval);
        return ret;
    }
    static_cast<PairValue&>(*rightval).pushToVector(ret);
    return ret;
}

ValuePtr PairValue::getLeft(){
    return this->leftval;
}

ValuePtr PairValue::getRight(){
    return this->rightval;
}

BuiltinProcValue::BuiltinProcValue(BuiltinFuncType* func) : func{func} {}

std::string BuiltinProcValue::toString() const{
    return "#<procedure>";
}

ValuePtr BuiltinProcValue::call(std::vector<ValuePtr>& args, EvalEnv& env){
    return this->func(args, env);
}

LambdaValue::LambdaValue(ValuePtr argparams, ValuePtr argbody, std::shared_ptr<EvalEnv> env) : parent{env} {
    auto params = argparams->toVector();
    for(auto item : params){
        if(auto name = item->asSymbol()){
            this->params.push_back(*name);
        }
        else{
            throw LispError("Invalid params");
        }
    }
    auto body = argbody->toVector();
    for(auto item : body){
        this->body.push_back(item);
    }
}

ValuePtr LambdaValue::apply(const std::vector<ValuePtr>& args){
    if(args.size() != this->params.size()){
        throw LispError("Wrong number of args.");
    }
    auto env = this->parent->createChild(params, args);
    ValuePtr ret;
    for(auto item : body){
        ret = env->eval(item);
    }
    return ret;
}

std::string LambdaValue::toString() const {
    return "#<procedure>";
}