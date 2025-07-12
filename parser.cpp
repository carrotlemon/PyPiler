#include "parser.h"

namespace Parser {
    Parser::Parser(std::vector<Lexer::Token> tokens) : tokens(tokens) { };

    Lexer::Token *Parser::lookahead(size_t increment) {
        return this->index+increment < this->tokens.size() ? &tokens[index+increment] : &tokens.back();
    }

    Lexer::Token *Parser::get(size_t i) {
        return i < this->tokens.size() ? &tokens[i] : &tokens.back();
    }

    std::vector<Stmt> *Parser::parse(std::vector<Lexer::Token> tokens) {

        while(index < tokens.size()) {
             stmts.push_back(parse_stmt());
        }
        return &stmts;
    }

    Stmt Parser::parse_stmt() {
        Lexer::Token *curr = &tokens[index];
        if(curr->type == Lexer::TokenType::Def) {
                
        } else if(curr->type == Lexer::TokenType::Id) {

        } else if(curr->type == Lexer::TokenType::If) {

        } else if(curr->type == Lexer::TokenType::For) {

        } else if(curr->type == Lexer::TokenType::While) {

        } else if(curr->type == Lexer::TokenType::Return) {
            ++index;
            return StmtReturn(ExprPtr(&parse_expr()));
        } else if(curr->type == Lexer::TokenType::Continue) {
            return StmtContinue();
        } else if(curr->type == Lexer::TokenType::Break) {
            return StmtBreak();
        } else if(curr->type == Lexer::TokenType::Pass) {
            return StmtPass();
        } else {
            return std::monostate();
        }
    }
    
    Expr Parser::parse_expr(bool in_paren) {
        // Lexer::Token *curr = &tokens[index];
        // end of expression if
        // rpar (all types)
        // Newline if no lpar
        // comma
        while(true) {
            // get next token
            Lexer::Token *curr = get(index);
            Lexer::TokenType type = curr->type;
            ++index;
            // if its an end condition, return
            if (type == Lexer::TokenType::RPar ||
                !in_paren && curr->type == Lexer::TokenType::Newline) {
                break;
            } else if(type == Lexer::TokenType::LPar) { // add other paren
                Expr inside = parse_expr(true);
            }
            // literal
            else if(tokenLiteral.contains(type)) {
                Lexer::Token *next = lookahead();
                if(tokenBinop.contains(next->type)) {
                    Expr left = ExprLiteral(curr->literal);
                    return ExprBinop(ExprPtr(&left), *next, ExprPtr(&parse_expr()));
                }
                return ExprLiteral(next->literal);
            }
            // id
            else if(type == Lexer::TokenType::Id) {

            }
            // operators
            else if(curr->type == Lexer::TokenType::Star) {

            }
            // else, keep parsing current expression
            

            // 
        }


        

        // whenever !parens.is_empty(), we completely ignore Newlines
        // 

  

        // part of current expression
        // literal,
        
        // recurse a new expression
        // lpar, operator

        // operators
        // pemmddas
        // indexing, function, acces, expon, 
        // 9002,     9002,     9002,  9001   
        // mul, div, ddiv, mod, add, sub, bit shift, bit and, bit xor, bit or
        // 800, 800, 800,  800, 600, 600
        // in, not in, is, is not, comparisons

        // not3, and2, or1, if..else0, 
        // map: k, v = op, power level
        

        
    }

    void Parser::print_stmts() {
        
    }
}   
