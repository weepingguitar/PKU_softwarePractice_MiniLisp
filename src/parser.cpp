#include <cassert>

#include "parser.h"
#include "error.h"

Parser::Parser(std::deque<TokenPtr> tokens) : tokens{std::move(tokens)} {}

ValuePtr Parser::parse(){
    if (tokens.empty()) 
        throw SyntaxError("EOF");
    auto token = std::move(tokens[0]);
    tokens.pop_front();
    if(token->getType() == TokenType::NUMERIC_LITERAL){
        auto value = static_cast<NumericLiteralToken&>(*token).getValue();
        return std::make_shared<NumericValue>(value);
    }
    else if(token->getType() == TokenType::BOOLEAN_LITERAL){
        auto value = static_cast<BooleanLiteralToken&>(*token).getValue();
        return std::make_shared<BooleanValue>(value);
    }
    else if(token->getType() == TokenType::STRING_LITERAL){
        auto value = static_cast<StringLiteralToken&>(*token).getValue();
        return std::make_shared<StringValue>(value);
    }
    else if(token->getType() == TokenType::IDENTIFIER){
        auto value = static_cast<IdentifierToken&>(*token).getName();
        return std::make_shared<SymbolValue>(value);
    }
    else if(token->getType() == TokenType::LEFT_PAREN){
        return this->parseTails();
    }
    else if (token->getType() == TokenType::QUOTE) {
        return std::make_shared<PairValue>(
            std::make_shared<SymbolValue>("quote"),
            std::make_shared<PairValue>(this->parse(),
                                        std::make_shared<NilValue>()));
    }
    else if (token->getType() == TokenType::QUASIQUOTE) {
        return std::make_shared<PairValue>(
            std::make_shared<SymbolValue>("quote"),
            std::make_shared<PairValue>(this->parse(),
                                        std::make_shared<NilValue>()));
    }
    else if (token->getType() == TokenType::UNQUOTE) {
        return std::make_shared<PairValue>(
            std::make_shared<SymbolValue>("quote"),
            std::make_shared<PairValue>(this->parse(),
                                        std::make_shared<NilValue>()));
    }
    throw SyntaxError("Unimplemented");
}

ValuePtr Parser::parseTails(){
    if (tokens.empty()) 
        throw SyntaxError("EOF");
    if(tokens[0]->getType() == TokenType::RIGHT_PAREN){
        tokens.pop_front();
        return std::make_shared<NilValue>();
    }
    auto car = this->parse();
    if (tokens.empty()) 
        throw SyntaxError("EOF");
    if(tokens[0]->getType() == TokenType::DOT){
        tokens.pop_front();
        auto cdr = this->parse();
        if(tokens.empty()||tokens[0]->getType() != TokenType::RIGHT_PAREN)
            throw SyntaxError("Wrong syntax");
        tokens.pop_front();
        return std::make_shared<PairValue>(car, cdr);
    }
    else{
        auto cdr = this->parseTails();
        return std::make_shared<PairValue>(car, cdr);
    }
}