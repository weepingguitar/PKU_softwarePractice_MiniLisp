#ifndef PARSER_H
#define PARSER_H

#include <memory>
#include <deque>

#include "value.h"
#include "token.h"

class Parser{
    std::deque<TokenPtr> tokens;
public:
    Parser(std::deque<TokenPtr> tokens);
    ValuePtr parseTails();
    ValuePtr parse();
};

#endif