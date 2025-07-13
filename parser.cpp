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

    ExprPtr Parser::parse_var_expr() { // id int bool float str
        Lexer::Token *curr = lookahead(0);
        if(curr->type == Lexer::Type::Id) {
            ExprPtr expr = parse_callget_expr();
            if(expr) {
                return expr;
            } else {
                ++index;
                Expr res = ExprId(*curr);
                return ExprPtr(&res);
            }
        } else if(tokenLiteral.contains(curr->type)) {
            ++index;
            Expr res = ExprId(*curr);
            return ExprPtr(&res);
        } else {
            std::cerr << "what?" << std::endl;
            return nullptr;
        }
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
