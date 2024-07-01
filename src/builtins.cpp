#include <cmath>

#include "./builtins.h"
#include "checkLegal.h"
namespace builtin{
    ValuePtr add(const std::vector<ValuePtr>& params, EvalEnv& env) {
        double result = 0;
        for (const auto& i : params) {
            if (!i->isNumber()) {
                throw LispError("Cannot add a non-numeric value.");
            }
            result += i->asNumber();
        }
        return std::make_shared<NumericValue>(result);
    }

    ValuePtr print(const std::vector<ValuePtr>& params, EvalEnv& env){
        std::cout << (*params[0]).toString() << std::endl;
        return std::make_shared<NilValue>();
    }

    ValuePtr multiply(const std::vector<ValuePtr>& params, EvalEnv& env){
        std::vector<double> nums;
        for (int i = 0; i < params.size();i++){
            if(typeid(*params[i]) == typeid(NumericValue))
            {
                double temp = static_cast<NumericValue&>(*params[i]).getval();
                nums.push_back(temp);
            }
            else{
                throw LispError("TypeError");
            }
        }
        double ans = 1;
        for (int i = 0; i < nums.size();i++){
            ans *= nums[i];
        }
        return std::make_shared<NumericValue>(ans);
    }

    ValuePtr substract(const std::vector<ValuePtr>& params, EvalEnv& env){
        std::vector<double> nums;
        checkInputNumber(params, {1, 2});
        for (int i = 0; i < params.size();i++){
            if(typeid(*params[i]) == typeid(NumericValue))
            {
                double temp = static_cast<NumericValue&>(*params[i]).getval();
                nums.push_back(temp);
            }
            else{
                throw LispError("TypeError");
            }
        }
        if(params.size() == 1){
            return std::make_shared<NumericValue>(-nums[0]);
        }
        else if(params.size() == 2){
            return std::make_shared<NumericValue>(nums[0] - nums[1]);
        }
        return std::make_shared<NilValue>();
    }

    ValuePtr divide(const std::vector<ValuePtr>& params, EvalEnv& env){
        double top = 1;
        double down;
        checkInputNumber(params, {1, 2});
        if(params.size() == 1){
            if(typeid(*params[0]) != typeid(NumericValue)){
                throw LispError("Invalid input.");
            }
            down = static_cast<NumericValue&>(*params[0]).getval();
        }
        else {

            if(typeid(*params[0]) != typeid(NumericValue)
            ||typeid(*params[1]) != typeid(NumericValue)){
                throw LispError("Invalid input.");
            }
            top = static_cast<NumericValue&>(*params[0]).getval();
            down = static_cast<NumericValue&>(*params[1]).getval();
        }
        if(down == 0){
            throw LispError("Divided by zero.");
        }
        return std::make_shared<NumericValue>(top / down);
    }

    ValuePtr greater(const std::vector<ValuePtr>& params, EvalEnv& env){
        checkInputNumber(params, {2});
        std::vector<double> nums;
        for (int i = 0; i < 2;i++) {
            if(typeid(*params[i]) == typeid(NumericValue))
            {
                double temp = static_cast<NumericValue&>(*params[i]).getval();
                nums.push_back(temp);
            }
            else{
                throw LispError("TypeError");
            }
        }
        return std::make_shared<BooleanValue>(nums[0] > nums[1]);
    }

    ValuePtr absfunc(const std::vector<ValuePtr>& params, EvalEnv& env){
        checkInputNumber(params, {1});
        if(typeid(*params[0]) != typeid(NumericValue)){
            throw LispError("Invalid input.");
        }
        double val = static_cast<NumericValue&>(*params[0]).getval();
        return std::make_shared<NumericValue>(val > 0 ? val : -val);
    }

    ValuePtr expt(const std::vector<ValuePtr>& params, EvalEnv& env){
        checkInputNumber(params, {2});
        if(typeid(*params[0]) != typeid(NumericValue)
        ||typeid(*params[1]) != typeid(NumericValue)){
            throw LispError("Invalid input.");
        }
        double x = static_cast<NumericValue&>(*params[0]).getval();
        double y = static_cast<NumericValue&>(*params[1]).getval();
        return std::make_shared<NumericValue>(std::pow(x, y));
    }

    ValuePtr quotient(const std::vector<ValuePtr>& params, EvalEnv& env){
        return std::make_shared<NumericValue>(std::trunc(static_cast<NumericValue&>(*(divide(params, env)))
                                                        .getval()));
    }


    ValuePtr applyfunc(const std::vector<ValuePtr>& params, EvalEnv& env){
        checkInputNumber(params, {2});
        auto pro = params[0];
        if(typeid(*params[1]) != typeid(PairValue)){
            throw LispError("Input error");
        }
        auto args = params[1]->toVector();
        return env.apply(pro, args);
    }

    ValuePtr display(const std::vector<ValuePtr>& params, EvalEnv& env){
        checkInputNumber(params, {1});
        if(typeid(*params[0]) == typeid(StringValue)){
            std::cout << static_cast<StringValue&>(*params[0]).getString()<<std::endl;
        }
        else
        {
            std::cout << "'" << params[0]->toString() << std::endl;
        }
        return std::make_shared<NilValue>();
    }


    ValuePtr error(const std::vector<ValuePtr>& params, EvalEnv& env){
        checkInputNumber(params, {0, 1});
        if(typeid(*params[0]) != typeid(StringValue)){
            throw LispError("Wrong type of input");
        }
        throw LispError(static_cast<StringValue&>(*params[0]).getString());
    }

    ValuePtr eval(const std::vector<ValuePtr>& params, EvalEnv& env){
        checkInputNumber(params, {1});
        return env.eval(params[0]);
    }

    ValuePtr exitLisp(const std::vector<ValuePtr>& params, EvalEnv& env){
        checkInputNumber(params, {0, 1});
        if(params.size() == 1){
            if(typeid(*params[0])!=typeid(NumericValue)){
                throw LispError("type error");
            }
            std::exit(static_cast<NumericValue&>(*params[0]).getval());
        }
        std::exit(0);
    }

    ValuePtr newline(const std::vector<ValuePtr>& params, EvalEnv& env){
        checkInputNumber(params, {0});
        std::cout << std::endl;
        return std::make_shared<NilValue>();
    }

    ValuePtr displayln(const std::vector<ValuePtr>& params, EvalEnv& env){
        display(params, env);
        newline({}, env);
        return std::make_shared<NilValue>();
    }

    ValuePtr atomcheck(const std::vector<ValuePtr>& params, EvalEnv& env){
        checkInputNumber(params, {1});
        if(typeid(*params[0]) == typeid(BooleanValue)
        ||typeid(*params[0]) == typeid(NumericValue)
        ||typeid(*params[0]) == typeid(StringValue)
        ||typeid(*params[0]) == typeid(SymbolValue)
        ||typeid(*params[0]) == typeid(NilValue))
        {
            return std::make_shared<BooleanValue>(1);
        }
        return std::make_shared<BooleanValue>(0);
    }

    ValuePtr booleancheck(const std::vector<ValuePtr>& params, EvalEnv& env){
        checkInputNumber(params, {1});
        return std::make_shared<BooleanValue>(typeid(*params[0]) ==
                                            typeid(BooleanValue));
    }

    ValuePtr intcheck(const std::vector<ValuePtr>& params, EvalEnv& env){
        checkInputNumber(params, {1});
        if(typeid(*params[0]) != typeid(NumericValue)){
            return std::make_shared<BooleanValue>(0);
        }
        double val = static_cast<NumericValue&>(*params[0]).getval();
        double intpart;
        return std::make_shared<BooleanValue>(modf(val, &intpart) == 0.0);
    }

    ValuePtr listcheck(const std::vector<ValuePtr>& params, EvalEnv& env){
        checkInputNumber(params, {1});
        auto val = params[0];
        while(typeid(*val) == typeid(PairValue)){
            val = static_cast<PairValue&>(*val).getRight();
        }
        return std::make_shared<BooleanValue>(typeid(*val) == typeid(NilValue));
    }

    ValuePtr numbercheck(const std::vector<ValuePtr>& params, EvalEnv& env){
        checkInputNumber(params, {1});
        return std::make_shared<BooleanValue>(typeid(*params[0]) == typeid(NumericValue));
    }

    ValuePtr nilcheck(const std::vector<ValuePtr>& params, EvalEnv& env){
        checkInputNumber(params, {1});
        return std::make_shared<BooleanValue>(typeid(*params[0]) == typeid(NilValue));
    }

    ValuePtr paircheck(const std::vector<ValuePtr>& params, EvalEnv& env){
        checkInputNumber(params, {1});
        return std::make_shared<BooleanValue>(typeid(*params[0]) == typeid(PairValue));
    }

    ValuePtr procheck(const std::vector<ValuePtr>& params, EvalEnv& env){
        checkInputNumber(params, {1});
        return std::make_shared<BooleanValue>(
            typeid(*params[0]) == typeid(BuiltinProcValue) 
            ||typeid(*params[0]) == typeid(LambdaValue));
    }

    ValuePtr stringcheck(const std::vector<ValuePtr>& params, EvalEnv& env){
        checkInputNumber(params, {1});
        return std::make_shared<BooleanValue>(typeid(*params[0]) ==
                                            typeid(StringValue));
    }

    ValuePtr symbolcheck(const std::vector<ValuePtr>& params, EvalEnv& env){
        checkInputNumber(params, {1});
        return std::make_shared<BooleanValue>(typeid(*params[0]) ==
                                            typeid(SymbolValue));
    }

    ValuePtr modulo(const std::vector<ValuePtr>& params, EvalEnv& env){
        checkInputNumber(params, {2});
        if(intcheck({params[0]}, env)->toString() == "#f"
        ||intcheck({params[1]}, env)->toString() == "#f")
        {
        throw LispError("Input should be intergers");
        }
        int val1 = static_cast<NumericValue&>(*params[0]).getval();
        int val2 = static_cast<NumericValue&>(*params[1]).getval();
        int ret = val1 % val2;
        if(ret * val2 < 0){
            ret += val2;
        }
        return std::make_shared<NumericValue>(ret);
    }

    ValuePtr remainderfunc(const std::vector<ValuePtr>& params, EvalEnv& env){
        checkInputNumber(params, {2});
        if(intcheck({params[0]}, env)->toString() == "#f"
        ||intcheck({params[1]}, env)->toString() == "#f")
        {
        throw LispError("Input should be intergers");
        }
        int x = static_cast<NumericValue&>(*params[0]).getval();
        int y = static_cast<NumericValue&>(*params[1]).getval();
        return std::make_shared<NumericValue>(x - y * std::trunc(x / y));
    }

    ValuePtr append(const std::vector<ValuePtr>& params, EvalEnv& env){
        if(params.size() == 0){
            return std::make_shared<NilValue>();
        }
        std::vector<ValuePtr> app{};

        for(auto item : params){
            if(!listcheck({item}, env)){
                throw LispError("Params should be lists.");
            }
            auto temp{item->toVector()};
            app.insert(app.end(), temp.begin(), temp.end());
        }
        return std::make_shared<PairValue>(app);
    }

    ValuePtr car(const std::vector<ValuePtr>& params, EvalEnv& env){
        checkInputNumber(params, {1});
        if(typeid(*params[0])!=typeid(PairValue)){
            throw LispError("typeError!");
        }
        if(!paircheck(params, env)){
            throw LispError("Invalid input.");
        }
        return static_cast<PairValue&>(*params[0]).getLeft();
    }

    ValuePtr cdr(const std::vector<ValuePtr>& params, EvalEnv& env){
        checkInputNumber(params, {1});
        if(!paircheck(params, env)){
            throw LispError("Invalid input.");
        }
        return static_cast<PairValue&>(*params[0]).getRight();
    }

    ValuePtr cons(const std::vector<ValuePtr>& params, EvalEnv& env){
        checkInputNumber(params, {2});
        return std::make_shared<PairValue>(params[0], params[1]);
    }

    ValuePtr length(const std::vector<ValuePtr>& params, EvalEnv& env){
        listcheck(params, env);
        return std::make_shared<NumericValue>(params[0]->toVector().size());
    }

    ValuePtr list(const std::vector<ValuePtr>& params, EvalEnv& env){
        if(params.size() == 0){
            return std::make_shared<NilValue>();
        }
        return std::make_shared<PairValue>(params);
    }

    ValuePtr map(const std::vector<ValuePtr>& params, EvalEnv& env){
        checkInputNumber(params, {2});
        ValuePtr pro = params[0];
        std::vector<ValuePtr> ret = params[1]->toVector();
        for (int i = 0; i < ret.size();i++){
            std::vector<ValuePtr> temp{ret[i]};
            ret[i] = env.apply(pro, temp);
        }
        return std::make_shared<PairValue>(ret);
    }

    ValuePtr filter(const std::vector<ValuePtr>& params, EvalEnv& env){
        checkInputNumber(params, {2});
        ValuePtr pro = params[0];
        std::vector<ValuePtr> pre = params[1]->toVector();


        std::vector<ValuePtr> ret{};
        for (int i = 0; i < pre.size();i++){
            std::vector<ValuePtr> temp{pre[i]};
            ValuePtr appliedval = env.apply(pro, temp);
            if(appliedval->toString() != "#f"){
                ret.push_back(pre[i]);
            }
        }
        
        if(ret.size() == 0){
            return std::make_shared<NilValue>();
        }

        return std::make_shared<PairValue>(ret);
    }

    ValuePtr reduce(const std::vector<ValuePtr>& params, EvalEnv& env){
        checkInputNumber(params, {2});
        listcheck({params[1]}, env);
        auto proc = params[0];
        auto vals = params[1]->toVector();
        if(vals.size() == 0){
            throw LispError("Input error");
        }
        if(vals.size() == 1){
            return vals[0];
        }
        std::vector<ValuePtr> temp{vals[0], reduce({proc, cdr({params[1]}, env)}, env)};
        return env.apply(proc, temp);
    }

    ValuePtr equal(const std::vector<ValuePtr>& params, EvalEnv& env){
        checkInputNumber(params, {2});
        if(typeid(*params[0]) != typeid(*params[1])){
            return std::make_shared<BooleanValue>(0);
        }
        return std::make_shared<BooleanValue>(params[0]->toString() == params[1]->toString());
    }

    ValuePtr eq(const std::vector<ValuePtr>& params, EvalEnv& env){
        checkInputNumber(params, {2});
        if(typeid(*params[0]) == typeid(*params[1])
        &&atomcheck({params[0]}, env)->toString() == "#t"){
            return std::make_shared<BooleanValue>(equal(params, env)->toString() ==
                                                "#t");
        }

        return std::make_shared<BooleanValue>(params[0] == params[1]);
    }

    ValuePtr notfunc(const std::vector<ValuePtr>& params, EvalEnv& env){
        checkInputNumber(params, {1});
        return std::make_shared<BooleanValue>(params[0]->toString() == "#f");
    }

    ValuePtr eqnum(const std::vector<ValuePtr>& params, EvalEnv& env){
        checkInputNumber(params, {2});
        if(typeid(*params[0]) != typeid(NumericValue)
        || typeid(*params[1]) != typeid(NumericValue))
        {
            throw LispError("Input must be numbers!");
        }
        double val1 = static_cast<NumericValue&>(*params[0]).getval();
        double val2 = static_cast<NumericValue&>(*params[1]).getval();
        return std::make_shared<BooleanValue>(val1 == val2);
    }

    ValuePtr less(const std::vector<ValuePtr>& params, EvalEnv& env){
        checkInputNumber(params, {2});
        std::vector<double> nums;
        for (int i = 0; i < 2;i++) {
            if(typeid(*params[i]) == typeid(NumericValue))
            {
                double temp = static_cast<NumericValue&>(*params[i]).getval();
                nums.push_back(temp);
            }
            else{
                throw LispError("TypeError");
            }
        }
        return std::make_shared<BooleanValue>(nums[0] < nums[1]);
    }

    ValuePtr lessOrEqual(const std::vector<ValuePtr>& params, EvalEnv& env){
        checkInputNumber(params, {2});
        std::vector<double> nums;
        for (int i = 0; i < 2;i++) {
            if(typeid(*params[i]) == typeid(NumericValue))
            {
                double temp = static_cast<NumericValue&>(*params[i]).getval();
                nums.push_back(temp);
            }
            else{
                throw LispError("TypeError");
            }
        }
        return std::make_shared<BooleanValue>(nums[0] <= nums[1]);
    }

    ValuePtr greaterOrEqual(const std::vector<ValuePtr>& params, EvalEnv& env){
        checkInputNumber(params, {2});
        std::vector<double> nums;
        for (int i = 0; i < 2;i++) {
            if(typeid(*params[i]) == typeid(NumericValue))
            {
                double temp = static_cast<NumericValue&>(*params[i]).getval();
                nums.push_back(temp);
            }
            else{
                throw LispError("TypeError");
            }
        }
        return std::make_shared<BooleanValue>(nums[0] >= nums[1]);
    }

    ValuePtr even(const std::vector<ValuePtr>& params, EvalEnv& env){
        if(intcheck(params, env)->toString()=="#f")
        {
            throw LispError("typeError!");
        }
        int val = static_cast<NumericValue&>(*params[0]).getval();
        return std::make_shared<BooleanValue>(val % 2==0);
    }

    ValuePtr odd(const std::vector<ValuePtr>& params, EvalEnv& env){
        if(intcheck(params, env)->toString()=="#f")
        {
            throw LispError("typeError!");
        }
        int val = static_cast<NumericValue&>(*params[0]).getval();
        return std::make_shared<BooleanValue>(val % 2!=0);
    }

    ValuePtr zero(const std::vector<ValuePtr>& params, EvalEnv& env){
        checkInputNumber(params, {1});
        return std::make_shared<BooleanValue>(params[0]->toString() == "0"
                                            ||params[0]->toString()== "-0");
    }

    ValuePtr stringToInt(const std::vector<ValuePtr>& params, EvalEnv& env){
        checkInputNumber(params, {1});
        if(typeid(*params[0])!=typeid(StringValue)){
            throw LispError("typeError!");
        }
        int num = std::stoi(static_cast<StringValue&>(*params[0]).getString());
        return std::make_shared<NumericValue>(num);
    }

    ValuePtr concat(const std::vector<ValuePtr>& params, EvalEnv& env){
        checkInputNumber(params, {2});
        if(typeid(*params[0])!=typeid(StringValue)
        ||typeid(*params[1])!=typeid(StringValue)){
            throw LispError("typeError!");
        }
        std::string ret = static_cast<StringValue&>(*params[0]).getString() +
                        static_cast<StringValue&>(*params[1]).getString();

        return std::make_shared<StringValue>(ret);
    }

    ValuePtr intToString(const std::vector<ValuePtr>& params, EvalEnv& env){
        checkInputNumber(params, {1});
        if(typeid(*params[0])!=typeid(NumericValue)){
            throw LispError("typeError!");
        }
        return std::make_shared<StringValue>(params[0]->toString());
    }

    ValuePtr at(const std::vector<ValuePtr>& params, EvalEnv& env){
        checkInputNumber(params, {2});
        if(!stringcheck({params[0]}, env)
        ||!intcheck({params[1]}, env)){
            throw LispError("typeError!");
        }

        int pos = static_cast<NumericValue&>(*params[1]).getval();
        std::string ret(1, (static_cast<StringValue&>(*params[0]).getString()[pos]));
        return std::make_shared<StringValue>(ret);
    }

    ValuePtr arcos(const std::vector<ValuePtr>& params, EvalEnv& env){
        checkInputNumber(params, {1});
        if(!numbercheck(params, env)){
            throw LispError("typeError!");
        }
        double num = static_cast<NumericValue&>(*params[0]).getval();
        return std::make_shared<NumericValue>(std::acos(num));
    }

    ValuePtr arcosh(const std::vector<ValuePtr>& params, EvalEnv& env){
        checkInputNumber(params, {1});
        if(!numbercheck(params, env)){
            throw LispError("typeError!");
        }
        double num = static_cast<NumericValue&>(*params[0]).getval();
        return std::make_shared<NumericValue>(std::acosh(num));
    }

    ValuePtr arsin(const std::vector<ValuePtr>& params, EvalEnv& env){
        checkInputNumber(params, {1});
        if(!numbercheck(params, env)){
            throw LispError("typeError!");
        }
        double num = static_cast<NumericValue&>(*params[0]).getval();
        return std::make_shared<NumericValue>(std::asin(num));
    }

    ValuePtr arsinh(const std::vector<ValuePtr>& params, EvalEnv& env){
        checkInputNumber(params, {1});
        if(!numbercheck(params, env)){
            throw LispError("typeError!");
        }
        double num = static_cast<NumericValue&>(*params[0]).getval();
        return std::make_shared<NumericValue>(std::asinh(num));
    }

    ValuePtr artan(const std::vector<ValuePtr>& params, EvalEnv& env){
        checkInputNumber(params, {1});
        if(!numbercheck(params, env)){
            throw LispError("typeError!");
        }
        double num = static_cast<NumericValue&>(*params[0]).getval();
        return std::make_shared<NumericValue>(std::atan(num));
    }

    ValuePtr artanh(const std::vector<ValuePtr>& params, EvalEnv& env){
        checkInputNumber(params, {1});
        if(!numbercheck(params, env)){
            throw LispError("typeError!");
        }
        double num = static_cast<NumericValue&>(*params[0]).getval();
        return std::make_shared<NumericValue>(std::atanh(num));
    }

    ValuePtr cbroot(const std::vector<ValuePtr>& params, EvalEnv& env){
        checkInputNumber(params, {1});
        if(!numbercheck(params, env)){
            throw LispError("typeError!");
        }
        double num = static_cast<NumericValue&>(*params[0]).getval();
        return std::make_shared<NumericValue>(std::cbrt(num));
    }

    ValuePtr ceil(const std::vector<ValuePtr>& params, EvalEnv& env){
        checkInputNumber(params, {1});
        if(!numbercheck(params, env)){
            throw LispError("typeError!");
        }
        double num = static_cast<NumericValue&>(*params[0]).getval();
        return std::make_shared<NumericValue>(std::ceil(num));
    }

    ValuePtr cos(const std::vector<ValuePtr>& params, EvalEnv& env){
        checkInputNumber(params, {1});
        if(!numbercheck(params, env)){
            throw LispError("typeError!");
        }
        double num = static_cast<NumericValue&>(*params[0]).getval();
        return std::make_shared<NumericValue>(std::cos(num));
    }

    ValuePtr cosh(const std::vector<ValuePtr>& params, EvalEnv& env){
        checkInputNumber(params, {1});
        if(!numbercheck(params, env)){
            throw LispError("typeError!");
        }
        double num = static_cast<NumericValue&>(*params[0]).getval();
        return std::make_shared<NumericValue>(std::cosh(num));
    }

    ValuePtr exp(const std::vector<ValuePtr>& params, EvalEnv& env){
        checkInputNumber(params, {1});
        if(!numbercheck(params, env)){
            throw LispError("typeError!");
        }
        double num = static_cast<NumericValue&>(*params[0]).getval();
        return std::make_shared<NumericValue>(std::exp(num));
    }

    ValuePtr exp2(const std::vector<ValuePtr>& params, EvalEnv& env){
        checkInputNumber(params, {1});
        if(!numbercheck(params, env)){
            throw LispError("typeError!");
        }
        double num = static_cast<NumericValue&>(*params[0]).getval();
        return std::make_shared<NumericValue>(std::exp2(num));
    }

    ValuePtr floor(const std::vector<ValuePtr>& params, EvalEnv& env){
        checkInputNumber(params, {1});
        if(!numbercheck(params, env)){
            throw LispError("typeError!");
        }
        double num = static_cast<NumericValue&>(*params[0]).getval();
        return std::make_shared<NumericValue>(std::floor(num));
    }

    ValuePtr log(const std::vector<ValuePtr>& params, EvalEnv& env){
        checkInputNumber(params, {1});
        if(!numbercheck(params, env)){
            throw LispError("typeError!");
        }
        double num = static_cast<NumericValue&>(*params[0]).getval();
        return std::make_shared<NumericValue>(std::log(num));
    }

    ValuePtr log10(const std::vector<ValuePtr>& params, EvalEnv& env){
        checkInputNumber(params, {1});
        if(!numbercheck(params, env)){
            throw LispError("typeError!");
        }
        double num = static_cast<NumericValue&>(*params[0]).getval();
        return std::make_shared<NumericValue>(std::log10(num));
    }

    ValuePtr log2(const std::vector<ValuePtr>& params, EvalEnv& env){
        checkInputNumber(params, {1});
        if(!numbercheck(params, env)){
            throw LispError("typeError!");
        }
        double num = static_cast<NumericValue&>(*params[0]).getval();
        return std::make_shared<NumericValue>(std::log2(num));
    }

    ValuePtr round(const std::vector<ValuePtr>& params, EvalEnv& env){
        checkInputNumber(params, {1});
        if(!numbercheck(params, env)){
            throw LispError("typeError!");
        }
        double num = static_cast<NumericValue&>(*params[0]).getval();
        return std::make_shared<NumericValue>(std::round(num));
    }

    ValuePtr sin(const std::vector<ValuePtr>& params, EvalEnv& env){
        checkInputNumber(params, {1});
        if(!numbercheck(params, env)){
            throw LispError("typeError!");
        }
        double num = static_cast<NumericValue&>(*params[0]).getval();
        return std::make_shared<NumericValue>(std::sin(num));
    }

    ValuePtr sinh(const std::vector<ValuePtr>& params, EvalEnv& env){
        checkInputNumber(params, {1});
        if(!numbercheck(params, env)){
            throw LispError("typeError!");
        }
        double num = static_cast<NumericValue&>(*params[0]).getval();
        return std::make_shared<NumericValue>(std::sinh(num));
    }

    ValuePtr sqrt(const std::vector<ValuePtr>& params, EvalEnv& env){
        checkInputNumber(params, {1});
        if(!numbercheck(params, env)){
            throw LispError("typeError!");
        }
        double num = static_cast<NumericValue&>(*params[0]).getval();
        return std::make_shared<NumericValue>(std::sqrt(num));
    }

    ValuePtr tan(const std::vector<ValuePtr>& params, EvalEnv& env){
        checkInputNumber(params, {1});
        if(!numbercheck(params, env)){
            throw LispError("typeError!");
        }
        double num = static_cast<NumericValue&>(*params[0]).getval();
        return std::make_shared<NumericValue>(std::tan(num));
    }

    ValuePtr tanh(const std::vector<ValuePtr>& params, EvalEnv& env){
        checkInputNumber(params, {1});
        if(!numbercheck(params, env)){
            throw LispError("typeError!");
        }
        double num = static_cast<NumericValue&>(*params[0]).getval();
        return std::make_shared<NumericValue>(std::tanh(num));
    }

    ValuePtr trunc(const std::vector<ValuePtr>& params, EvalEnv& env){
        checkInputNumber(params, {1});
        if(!numbercheck(params, env)){
            throw LispError("typeError!");
        }
        double num = static_cast<NumericValue&>(*params[0]).getval();
        return std::make_shared<NumericValue>(std::trunc(num));
    }

    ValuePtr pow(const std::vector<ValuePtr>& params, EvalEnv& env){
        checkInputNumber(params, {2});
        if(!numbercheck({params[0]}, env)
        ||!numbercheck({params[1]}, env)){
            throw LispError("typeError!");
        }
        double num1 = static_cast<NumericValue&>(*params[0]).getval();
        double num2 = static_cast<NumericValue&>(*params[1]).getval();
        return std::make_shared<NumericValue>(std::pow(num1, num2));
    }

}

using namespace builtin;

const std::unordered_map<std::string, ValuePtr> BuiltinFuncs{
    {"+", std::make_shared<BuiltinProcValue>(&add)},
    {"print", std::make_shared<BuiltinProcValue>(&print)},
    {"*", std::make_shared<BuiltinProcValue>(&multiply)},
    {">", std::make_shared<BuiltinProcValue>(&greater)},
    {"-", std::make_shared<BuiltinProcValue>(&substract)},
    {"apply", std::make_shared<BuiltinProcValue>(&applyfunc)},
    {"display", std::make_shared<BuiltinProcValue>(&display)},
    {"displayln", std::make_shared<BuiltinProcValue>(&displayln)},
    {"exit", std::make_shared<BuiltinProcValue>(&exitLisp)},
    {"error", std::make_shared<BuiltinProcValue>(&error)},
    {"eval", std::make_shared<BuiltinProcValue>(&eval)},
    {"newline", std::make_shared<BuiltinProcValue>(&newline)},
    {"atom?", std::make_shared<BuiltinProcValue>(&atomcheck)},
    {"boolean?", std::make_shared<BuiltinProcValue>(&booleancheck)},
    {"integer?", std::make_shared<BuiltinProcValue>(&intcheck)},
    {"list?", std::make_shared<BuiltinProcValue>(&listcheck)},
    {"number?", std::make_shared<BuiltinProcValue>(&numbercheck)},
    {"null?", std::make_shared<BuiltinProcValue>(&nilcheck)},
    {"pair?", std::make_shared<BuiltinProcValue>(&paircheck)},
    {"procedure?", std::make_shared<BuiltinProcValue>(&procheck)},
    {"string?", std::make_shared<BuiltinProcValue>(&stringcheck)},
    {"symbol?", std::make_shared<BuiltinProcValue>(&symbolcheck)},
    {"append", std::make_shared<BuiltinProcValue>(&append)},
    {"car", std::make_shared<BuiltinProcValue>(&car)},
    {"cdr", std::make_shared<BuiltinProcValue>(&cdr)},
    {"cons", std::make_shared<BuiltinProcValue>(&cons)},
    {"length", std::make_shared<BuiltinProcValue>(&length)},
    {"list", std::make_shared<BuiltinProcValue>(&list)},
    {"map", std::make_shared<BuiltinProcValue>(&map)},
    {"filter", std::make_shared<BuiltinProcValue>(&filter)},
    {"reduce", std::make_shared<BuiltinProcValue>(&reduce)},
    {"/", std::make_shared<BuiltinProcValue>(&divide)},
    {"abs", std::make_shared<BuiltinProcValue>(&absfunc)},
    {"expt", std::make_shared<BuiltinProcValue>(&expt)},
    {"quotient", std::make_shared<BuiltinProcValue>(&quotient)},
    {"modulo", std::make_shared<BuiltinProcValue>(&modulo)},
    {"remainder", std::make_shared<BuiltinProcValue>(&remainderfunc)},
    {"equal?", std::make_shared<BuiltinProcValue>(&equal)},
    {"eq?", std::make_shared<BuiltinProcValue>(&eq)},
    {"not", std::make_shared<BuiltinProcValue>(&notfunc)},
    {"=", std::make_shared<BuiltinProcValue>(&eqnum)},
    {"<", std::make_shared<BuiltinProcValue>(&less)},
    {"<=", std::make_shared<BuiltinProcValue>(&lessOrEqual)},
    {">=", std::make_shared<BuiltinProcValue>(&greaterOrEqual)},
    {"even?", std::make_shared<BuiltinProcValue>(&even)},
    {"odd?", std::make_shared<BuiltinProcValue>(&odd)},
    {"zero?", std::make_shared<BuiltinProcValue>(&zero)},
    {"to_int", std::make_shared<BuiltinProcValue>(&stringToInt)},
    {"to_string", std::make_shared<BuiltinProcValue>(&intToString)},
    {"concat", std::make_shared<BuiltinProcValue>(&concat)},
    {"at", std::make_shared<BuiltinProcValue>(&at)},
    {"arcos", std::make_shared<BuiltinProcValue>(&arcos)},
    {"arcosh", std::make_shared<BuiltinProcValue>(&arcosh)},
    {"arsin", std::make_shared<BuiltinProcValue>(&arsin)},
    {"arsinh", std::make_shared<BuiltinProcValue>(&arsinh)},
    {"artan", std::make_shared<BuiltinProcValue>(&artan)},
    {"artanh", std::make_shared<BuiltinProcValue>(&artanh)},
    {"cbrt", std::make_shared<BuiltinProcValue>(&cbroot)},
    {"ceil", std::make_shared<BuiltinProcValue>(&builtin::ceil)},
    {"cos", std::make_shared<BuiltinProcValue>(&builtin::cos)},
    {"cosh", std::make_shared<BuiltinProcValue>(&builtin::cosh)},
    {"exp", std::make_shared<BuiltinProcValue>(&builtin::exp)},
    {"exp2", std::make_shared<BuiltinProcValue>(&builtin::exp2)},
    {"floor", std::make_shared<BuiltinProcValue>(&builtin::floor)},
    {"log", std::make_shared<BuiltinProcValue>(&builtin::log)},
    {"log10", std::make_shared<BuiltinProcValue>(&builtin::log10)},
    {"log2", std::make_shared<BuiltinProcValue>(&builtin::log2)},
    {"round", std::make_shared<BuiltinProcValue>(&builtin::round)},
    {"sin", std::make_shared<BuiltinProcValue>(&builtin::sin)},
    {"sinh", std::make_shared<BuiltinProcValue>(&builtin::sinh)},
    {"sqrt", std::make_shared<BuiltinProcValue>(&builtin::sqrt)},
    {"tan", std::make_shared<BuiltinProcValue>(&builtin::tan)},
    {"tanh", std::make_shared<BuiltinProcValue>(&builtin::tanh)},
    {"trunc", std::make_shared<BuiltinProcValue>(&builtin::trunc)},
};