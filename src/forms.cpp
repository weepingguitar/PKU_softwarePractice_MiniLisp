#include "forms.h"
#include "error.h"

ValuePtr defineForm(ValuePtr& arg, EvalEnv& env) {
    auto leftval = static_cast<PairValue&>(*arg).getLeft();
    auto rightval = static_cast<PairValue&>(*arg).getRight();
    if(auto name = leftval->asSymbol()){
        auto vals = env.evalList(rightval);
        ValuePtr val;
        if(vals.size() == 0){
            throw LispError("Syntax error");
        }
        else if(vals.size() == 1){
            val = vals[0];
        }
        else{
            val = env.eval(rightval);
        }
        return env.addValue(*name, val);
    }
    else if(typeid(*leftval) == typeid(PairValue)){
        if(auto name = static_cast<PairValue&>(*leftval).getLeft()->asSymbol())
        {
            auto argparams = static_cast<PairValue&>(*leftval).getRight();
            return env.addValue(
                *name, std::make_shared<LambdaValue>(argparams, rightval, env.shared_from_this()));
        }
        else{
            throw LispError("Invalid name.");
        }
    }
    else{
        throw LispError("Unimplemented");
    }
    return std::make_shared<NilValue>();
}

ValuePtr quote(ValuePtr& arg, EvalEnv& env){
    ValuePtr val = static_cast<PairValue&>(*arg).getLeft();
    return val;
}

ValuePtr ifForm(ValuePtr& arg, EvalEnv& env){
    auto args = arg->toVector();
    ValuePtr condition = env.eval(args[0]);
    if(condition->toString() == "#f"){
        return env.eval(args[2]);
    }
    else{
        return env.eval(args[1]);
    }
}

ValuePtr andForm(ValuePtr& arg, EvalEnv& env){
    ValuePtr val = std::make_shared<BooleanValue>(1);
    auto args = arg->toVector();
    for(ValuePtr item : args){
        ValuePtr tempval = env.eval(item);
        if(tempval->toString() == "#f"){
            return std::make_shared <BooleanValue>(0);
        }
        else{
            val = tempval;
        }
    }
    return val;
}

ValuePtr orForm(ValuePtr& arg, EvalEnv& env){
    ValuePtr val = std::make_shared<BooleanValue>(0);
    auto args = arg->toVector();
    for(ValuePtr item : args){
        ValuePtr tempval = env.eval(item);
        if(tempval->toString() != "#f"){
            return tempval;
        }
    }
    return val;
}

ValuePtr lambdaForm(ValuePtr& arg, EvalEnv& env){
    auto leftval = static_cast<PairValue&>(*arg).getLeft();
    auto rightval = static_cast<PairValue&>(*arg).getRight();
    return std::make_shared<LambdaValue>(leftval, rightval, env.shared_from_this());
}

ValuePtr condform(ValuePtr& arg, EvalEnv& env){
    auto clauses = arg->toVector();
    ValuePtr ret = std::make_shared<NilValue>();
    for (int i = 0; i < clauses.size();i++) {
        ValuePtr item = clauses[i];
        if(typeid(*item) != typeid(PairValue)){
            throw LispError("Clauses should be pairs");
        }

        ValuePtr cond = static_cast<PairValue&>(*item).getLeft();
        auto exprs = static_cast<PairValue&>(*item).getRight()->toVector();
        
        if(auto name = cond->asSymbol()){
            if((*name) == "else"){
                if(i!=clauses.size() - 1){
                    throw LispError("Else should appear in the last clause.");
                }
                for(auto expr : exprs){
                    ret = env.eval(expr);
                }
                return ret;
            }
        }
        
        if(env.eval(cond) -> toString() != "#f") {
            ret = env.eval(cond);
            for(auto expr : exprs){
                ret = env.eval(expr);
            }
            break;
        }
    }
    return ret;
}

ValuePtr beginform(ValuePtr& arg, EvalEnv& env){
    auto exprs = arg->toVector();
    ValuePtr ret = std::make_shared<NilValue>();
    for(auto expr : exprs){
        ret = env.eval(expr);
    }
    return ret;
}

ValuePtr letform(ValuePtr& arg, EvalEnv& env){
    auto args = arg->toVector();
    auto bindings = args[0]->toVector();

    std::vector<ValuePtr> names;
    std::vector<ValuePtr> vals;

    for(auto binding : bindings){
        if(!(typeid(*binding) == typeid(PairValue)))
        {
            throw LispError("Binding should be pair.");
        }
        auto pair = binding->toVector();
        if(pair.size() != 2){
            throw LispError("Binding error");
        }
        names.push_back(pair[0]);
        vals.push_back(env.eval(pair[1]));
    }

    ValuePtr params = std::make_shared<PairValue>(names);

    args.erase(args.begin());
    ValuePtr body = std::make_shared<PairValue>(args);

    LambdaValue lam(params, body, env.shared_from_this());
    return lam.apply(vals);
}

ValuePtr quasiquote(ValuePtr& arg, EvalEnv& env){
    auto args = static_cast<PairValue&>(*arg).getLeft()->toVector();
    for(auto& item : args){
        if(typeid(*item) == typeid(PairValue)){
            ValuePtr left = static_cast<PairValue&>(*item).getLeft();
            ValuePtr right = static_cast<PairValue&>(*item).getRight();
            if(auto name = left->asSymbol()){
                if((*name) == "unquote"){
                    item = env.eval(static_cast<PairValue&>(*right).getLeft());
                }
            }
        }
    }
    return std::make_shared<PairValue>(args);
}

std::unordered_map<std::string, SpecialFormType*> SPECIAL_FORMS{
    {"define", defineForm},
    {"'", quote},
    {"quote", quote},
    {"if", ifForm},
    {"and", andForm},
    {"or", orForm},
    {"lambda", lambdaForm},
    {"cond", condform},
    {"begin", beginform},
    {"let", letform},
    {"`", quasiquote},
    {"quasiquote", quasiquote}
};