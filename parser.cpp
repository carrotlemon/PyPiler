#include "parser.h"

namespace Parser {
    Parser::Parser(std::vector<Lexer::Token> tokens) : tokens(tokens) { };

    Lexer::Token *Parser::lookahead(size_t increment) {
        return this->index+increment < this->tokens.size() ? &tokens[index+increment] : &tokens.back();
    }

    Lexer::Token *Parser::get(size_t i) {
        return i < this->tokens.size() ? &tokens[i] : &tokens.back();
    }

    std::vector<StmtPtr> *Parser::parse(std::vector<Lexer::Token> tokens) {

        while(index < tokens.size()) {
             stmts.push_back(parse_stmt());
        }
        return &stmts;
    }

    StmtPtr Parser::parse_stmt() {
        Lexer::Token *curr = &tokens[index];
        Stmt res;
        if(curr->type == Lexer::Type::Def) {
                
        } else if(curr->type == Lexer::Type::Id) {

        } else if(curr->type == Lexer::Type::If) {

        } else if(curr->type == Lexer::Type::For) {

        } else if(curr->type == Lexer::Type::While) {

        } else if(curr->type == Lexer::Type::Return) {
            ++index;
            res = StmtReturn(parse_expr());
        } else if(curr->type == Lexer::Type::Continue) {
            res = StmtContinue();
        } else if(curr->type == Lexer::Type::Break) {
            res = StmtBreak();
        } else if(curr->type == Lexer::Type::Pass) {
            res = StmtPass();
        } else {
            res = std::monostate();
        }
        return StmtPtr(&res);
    }
    
    ExprPtr Parser::parse_expr() {
        // Lexer::Token *curr = &tokens[index];


        // end of expression if
        // rpar (all types)
        // Newline if no lpar
        // comma
        // EOF


        // while(true) {
        //     // get next token
        //     Lexer::Token *curr = get(index);
        //     Lexer::Type type = curr->type;
        //     ++index;



        //     // if its an end condition, return
        //     if (tokenRParen.contains(type) ||
        //         !in_paren && curr->type == Lexer::Type::Newline ||
        //         type == Lexer::Type::Comma) {
        //         break;
        //     } else if(type == Lexer::Type::LPar) { // add other paren
        //         Expr inside = parse_expr(true);
        //     }


        //     // literal
        //     else if(tokenLiteral.contains(type) || type == Lexer::Type::Id || tokenUnop.contains(type)) {
        //         Lexer::Token *next = lookahead();


        //         if(tokenUnop.contains(type)) {
        //             // if paren then parseexpr else just next literal
        //             // -(2+2)
        //             if(next->type == Lexer::Type::LPar) {
        //                 return ExprUnop(*curr, ExprPtr(&parse_expr(true)));
        //             }
        //             ++index;
        //             Expr left = ExprLiteral(next->literal);
        //             return ExprUnop(*curr, ExprPtr(&left));
        //         }
                
                
        //         if(type == Lexer::Type::Id) {
        //             if(next->type == Lexer::Type::LPar) {

        //             } else if(next->type == Lexer::Type::LSquare) {

        //             } else if(next->type == Lexer::Type::Period) {
        //                 Expr left = ExprLiteral(curr->literal);
        //                 return ExprBinop(ExprPtr(&left), *next, ExprPtr(&parse_expr()));
        //             }
        //         } else if(tokenBinop.contains(next->type)) {
        //             Expr left = ExprLiteral(curr->literal);
        //             return ExprBinop(ExprPtr(&left), *next, ExprPtr(&parse_expr())); // how to add order of op?
        //         }
        //         return ExprLiteral(next->literal);
        //     }

        //     // 
        // }

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

    // ordered by operation precedence
    ExprPtr Parser::parse_paren_expr() {
        Lexer::Token *curr = lookahead(0);
        if(curr->type == Lexer::Type::LPar) {
            ++index;
            ExprPtr inside = parse_expr();
            ++index;
            return inside;
        }
        return nullptr;
    }
    ExprPtr Parser::parse_callget_expr() {
        Lexer::Token *next = lookahead();
        if(next->type == Lexer::Type::LPar) { // function call
            Expr res = ExprFunc();
            // get and set args
            return ExprPtr(&res);
        } else if(next->type == Lexer::Type::LSquare) { // []
            ++index;
            ++parenCount;
            Expr res = ExprIndex(*lookahead(0));
            while(lookahead(0)->type != Lexer::Type::RSquare) {
                // get is safe here because res is always an ExprIndex
                std::get<ExprIndex>(res).args.push_back(parse_expr());
                ++index; // skip the ':' separating args
            }
            ++index;
            --parenCount;
            return ExprPtr(&res);
        } else if(next->type == Lexer::Type::Period) { // .
            Lexer::Token *id = lookahead(0);
            Lexer::Token *field = lookahead(2);
            Expr left = ExprId(*id);
            Expr right = ExprId(*field);
            Expr res = ExprBinop(ExprPtr(&left), *next, ExprPtr(&right));
            index += 3;
            return ExprPtr(&res);
        } else {
            Lexer::Token *id = lookahead(0);
            Expr res = ExprId(*id);
            index += 2;
            return ExprPtr(&res);
        }
    }

    ExprPtr Parser::parse_var_expr() {

    }

    ExprPtr Parser::parse_pow_expr() {

    }

    ExprPtr Parser::parse_unop_expr() {
        Lexer::Token *curr = lookahead(0);
        if(tokenUnop.contains(curr->type)) { // ~ -
            ++index;
            Expr res = ExprUnop(*curr, parse_unop_expr());
            return ExprPtr(&res);
        }
        return parse_pow_expr();
    }

    ExprPtr Parser::parse_multdiv_expr() {
        Lexer::Token *curr = lookahead(0);

    }

    ExprPtr Parser::parse_addsub_expr() {

    }

    ExprPtr Parser::parse_bit_shift_expr() {

    }

    ExprPtr Parser::parse_bit_and_expr() {

    }

    ExprPtr Parser::parse_bit_xor_expr() {

    }

    ExprPtr Parser::parse_bit_or_expr() {

    }

    ExprPtr Parser::parse_cmp_expr() {

    }

    ExprPtr Parser::parse_not_expr() {

    }

    ExprPtr Parser::parse_and_expr() {

    }

    ExprPtr Parser::parse_or_expr() {

    }
}   
