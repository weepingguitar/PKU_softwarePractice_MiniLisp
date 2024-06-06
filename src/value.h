#ifndef VALUE_H
#define VALUE_H

#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <optional>

class EvalEnv;
class Value{
public:
    Value();
    bool isNumber();
    double asNumber();
    virtual std::string toString() const = 0;
    virtual std::vector<std::shared_ptr<Value>> toVector();
    virtual std::optional<std::string> asSymbol();
};

using ValuePtr = std::shared_ptr<Value>;

class BooleanValue:public Value{
    bool val;
public:
    BooleanValue(bool val);
    std::string toString() const;
    std::vector<ValuePtr> toVector();
};

class NumericValue:public Value{
    double val;
public:
    double getval();
    NumericValue(double val);
    std::string toString() const;
    std::vector<ValuePtr> toVector();
};

class StringValue:public Value{
    std::string val;
public:
    StringValue(const std::string& val);
    std::string toString() const;
    std::string getString();
    std::vector<ValuePtr> toVector();
};

class NilValue:public Value{
public:
    NilValue();
    std::string toString() const;
    std::vector<ValuePtr> toVector();
};

class SymbolValue:public Value{
    std::string val;
public:
    SymbolValue(const std::string& val);
    std::string toString() const;
    std::optional<std::string> asSymbol();
    std::vector<ValuePtr> toVector();
};

class PairValue:public Value{
    ValuePtr leftval;
    ValuePtr rightval;
public:
    PairValue(ValuePtr leftval, ValuePtr rightval);
    PairValue(const std::vector<ValuePtr>& params);
    std::string pair_toString(bool start) const;
    std::string toString() const;
    void pushToVector(std::vector<ValuePtr>& ret);
    std::vector<ValuePtr> toVector();
    ValuePtr getLeft();
    ValuePtr getRight();
};

using BuiltinFuncType = ValuePtr(const std::vector<ValuePtr>&, EvalEnv&);

class BuiltinProcValue : public Value{
    BuiltinFuncType* func;
public:
    BuiltinProcValue(BuiltinFuncType* func);
    std::string toString() const;
    ValuePtr call(std::vector<ValuePtr>& args, EvalEnv& env);
};

class LambdaValue : public Value{
    std::vector<std::string> params;
    std::vector<ValuePtr> body;
    std::shared_ptr<EvalEnv> parent;
public:
    LambdaValue(ValuePtr argparams, ValuePtr argbody, std::shared_ptr<EvalEnv> env);
    ValuePtr apply(const std::vector<ValuePtr>& args);
    std::string toString() const override;
};
#endif