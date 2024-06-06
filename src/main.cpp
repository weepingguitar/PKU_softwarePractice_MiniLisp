#include <iostream>
#include <string>
#include <fstream>

#include "./tokenizer.h"
#include "./value.h"
#include "./parser.h"
#include "rjsj_test.hpp"
#include "./eval_env.h"
#include "error.h"

using ValuePtr = std::shared_ptr<Value>; // 把这个添加到 value.h，可以减少许多重复的代码。

struct TestCtx {
    std::shared_ptr<EvalEnv> env = EvalEnv::createGlobal();
    std::string eval(std::string input) {
        auto tokens = Tokenizer::tokenize(input);
        Parser parser(std::move(tokens));
        auto value = parser.parse();
        auto result = env->eval(std::move(value));
        return result->toString();
    }
};

int main(int argc, char** argv) {
    if(argc == 1){
        std::shared_ptr<EvalEnv> env = EvalEnv::createGlobal();
        while (true) {
            try {
                std::cout << ">>> " ;
                std::string line;
                std::getline(std::cin, line);
                if (std::cin.eof()) {
                    std::exit(0);
                }
                auto tokens = Tokenizer::tokenize(line);
                Parser parser(std::move(tokens));
                auto value = parser.parse();
                auto result = env->eval(std::move(value));
                std::cout << result->toString() << std::endl;
            } catch (std::runtime_error& e) {
                std::cerr << "Error: " << e.what() << std::endl;
            }
        }
    }
    else if(argc == 2){
        std::shared_ptr<EvalEnv> env = EvalEnv::createGlobal();
        std::ifstream file{argv[1]};
        if(!file.is_open()){
            throw LispError("Open error.");
        }
        std::string line;
        while (std::getline(file, line)) {
            try {
                if(line == ""){
                    continue;
                }
                auto tokens = Tokenizer::tokenize(line);
                Parser parser(std::move(tokens));
                auto value = parser.parse();
                auto result = env->eval(std::move(value));
            } catch (std::runtime_error& e) {
                std::cerr << "Error: " << e.what() << std::endl;
            }
        }

    }
    else{
        throw LispError("Wrong number of argument.");
    }
}
