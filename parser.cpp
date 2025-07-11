#include "parser.h"

namespace Parser {
    Parser::Parser(std::vector<Lexer::Token> tokens) : tokens(tokens) { };

    Lexer::Token Parser::lookahead(int increment) {
        return this->index+increment < this->tokens.size() ? tokens[index+increment] : tokens.back();
    }

    std::vector<Stmt> Parser::parse(std::vector<Lexer::Token> tokens) {
        std::vector<Stmt> res;

        int index = 0;

        while(index < tokens.size()) {
            Lexer::Token *curr = &tokens[index];
            if(curr->type == Lexer::TokenType::Def) {

            } else if(curr->type == Lexer::TokenType::Id) {

            } else if(curr->type == Lexer::TokenType::If) {

            } else if(curr->type == Lexer::TokenType::For) {

            } else if(curr->type == Lexer::TokenType::While) {

            } else if(curr->type == Lexer::TokenType::Return) {

            } else if(curr->type == Lexer::TokenType::Continue) {

            } else if(curr->type == Lexer::TokenType::Break) {

            } else if(curr->type == Lexer::TokenType::Pass) {

            }
        }

        return res;
    }

    Stmt Parser::parse_stmt(size_t start, size_t end) {

    }

    Expr Parser::parse_expr(size_t start, size_t end) {

    }
}   
