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
        return parse_or_expr();
    }

    void Parser::print() {
        for(auto& stmt: stmts) {
            print_stmt(stmt);
        }
    }

    void Parser::print_stmt(StmtPtr stmt) {
        if (std::holds_alternative<StmtExpression>(*stmt)) {
            auto& stmtExpression = std::get<StmtExpression>(*stmt);
            std::cout << "StmtExpression" << std::endl;
            print_expr(stmtExpression.expr);
        } else if (std::holds_alternative<StmtAssign>(*stmt)) {
            auto& stmtAssign = std::get<StmtAssign>(*stmt);
            std::cout << "StmtAssign" << std::endl;
            std::cout << stmtAssign.name << " " << stmtAssign.op.lexeme << std::endl;
            print_expr(stmtAssign.expr);
        } else if (std::holds_alternative<StmtBlock>(*stmt)) {
            auto& stmtBlock = std::get<StmtBlock>(*stmt);
            std::cout << "StmtBlock" << std::endl;
            for (size_t i = 0; i < stmtBlock.stmts.size(); ++i) {
                print_stmt(stmtBlock.stmts[i]);
            }
        } else if (std::holds_alternative<StmtIf>(*stmt)) {
            auto& stmtIf = std::get<StmtIf>(*stmt);
            std::cout << "StmtIf" << std::endl;
            size_t numConditions = stmtIf.conditions.size();
            for (size_t i = 0; i < numConditions; ++i) {
                print_expr(stmtIf.conditions[i]);
                print_stmt(stmtIf.bodies[i]);
            }
            if (stmtIf.bodies.size() > numConditions) {
                print_stmt(stmtIf.bodies[numConditions]);
            }
        } else if (std::holds_alternative<StmtFor>(*stmt)) {
            auto& stmtFor = std::get<StmtFor>(*stmt);
            std::cout << "StmtFor" << std::endl;
            print_expr(stmtFor.condition);
            print_stmt(stmtFor.body);
        } else if (std::holds_alternative<StmtWhile>(*stmt)) {
            auto& stmtWhile = std::get<StmtWhile>(*stmt);
            std::cout << "StmtWhile" << std::endl;
            print_expr(stmtWhile.condition);
            print_stmt(stmtWhile.body);
        } else if (std::holds_alternative<StmtFunc>(*stmt)) {
            auto& stmtFunc = std::get<StmtFunc>(*stmt);
            std::cout << "StmtFunc" << std::endl;
            std::cout << stmtFunc.name << std::endl;
            for (size_t i = 0; i < stmtFunc.args.size(); ++i) {
                std::cout << type_to_string(stmtFunc.argTypes[i]) << " " << stmtFunc.args[i] << std::endl;
            }
            print_stmt(stmtFunc.body);
        } else if (std::holds_alternative<StmtReturn>(*stmt)) {
            auto& stmtReturn = std::get<StmtReturn>(*stmt);
            std::cout << "StmtReturn" << std::endl;   
            print_expr(stmtReturn.value);         
        } else if (std::holds_alternative<StmtContinue>(*stmt)) {
            std::cout << "StmtContinue" << std::endl;   
        } else if (std::holds_alternative<StmtBreak>(*stmt)) {
            std::cout << "StmtBreak" << std::endl;
        } else if (std::holds_alternative<StmtPass>(*stmt)) {
            std::cout << "StmtPass" << std::endl;
        } else {
            std::cout << "Stmt variant not found.";
        }
    }

    std::string type_to_string(TypeName type) {
        if(type == Int) { return "int"; }
        else if(type == Float) { return "float"; }
        else if(type == Bool) { return "bool"; }
        else if(type == NoneType) { return "NoneType"; }
        else if(type == List) { return "list"; }
        else if(type == Dict) { return "dict"; }
        else if(type == Tuple) { return "tuple"; }
        else if(type == Set) { return "set"; }
        else if(type == Range) { return "range"; }
        else if(type == Enumerate) { return "enumerate"; }
        else if(type == Zip) { return "zip"; }
        else if(type == Slice) { return "slice"; }
        else if(type == Frozenset) { return "frozenset"; }
        else if(type == Function) { return "function"; }
        else if(type == Lambda) { return "lambda"; }
        else if(type == Type) { return "type"; }
        else { return "you missed something bro"; }
    }

    // print_expr ends in a newline
    void Parser::print_expr(ExprPtr expr) {

    }

    // ordered by operation precedence
    ExprPtr Parser::parse_paren_expr() {
        Lexer::Token *curr = lookahead(0);
        if(curr->type == Lexer::Type::LPar) { // tuple or paren
            Expr res = ExprTuple();
            std::vector<ExprPtr> *elements = &std::get<ExprTuple>(res).elements;
            if(lookahead()->type != Lexer::Type::RPar) {
                while(lookahead(0)->type != Lexer::Type::RPar) {
                    ++index;
                    elements->push_back(parse_expr());
                }
                if(elements->size() == 1) {
                    // check for (expr,)
                    if(!elements->back()) {
                        elements->pop_back();
                    }
                    return (*elements)[0];
                }
            }
            return ExprPtr(&res);
        } else if(curr->type == Lexer::Type::LSquare) { // list
            Expr res = ExprList();
            std::vector<ExprPtr> *elements = &std::get<ExprList>(res).elements;
            if(lookahead()->type != Lexer::Type::RSquare) {
                while(lookahead(0)->type != Lexer::Type::RSquare) {
                    ++index;
                    elements->push_back(parse_expr());
                }
                if(elements->size() == 1) {
                    return (*elements)[0];
                }
            }
            return ExprPtr(&res);
        } else if(curr->type == Lexer::Type::LBrace) { // dict
            Expr res = ExprList();
            std::vector<ExprPtr> *elements = &std::get<ExprList>(res).elements;
            ++index;
            while(lookahead(0)->type != Lexer::Type::RSquare) {
                elements->push_back(parse_expr());
                ++index;
            }
            return ExprPtr(&res);
        }
        return nullptr;
    }

    ExprPtr Parser::parse_callget_expr() {
        Lexer::Token *next = lookahead(0);
        if(next->type == Lexer::Type::LPar) { // function call
            Expr res = ExprFunc();
            // get and set args
            return ExprPtr(&res);
        } else if(next->type == Lexer::Type::LSquare) { // []
            ++index;
            Expr res = ExprIndex(*lookahead(0));
            while(lookahead(0)->type != Lexer::Type::RSquare) {
                // get is safe here because res is always an ExprIndex
                std::get<ExprIndex>(res).args.push_back(parse_expr());
                ++index; // skip the ':' separating args
            }
            ++index;
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
            Expr res = ExprLiteral(*curr);
            return ExprPtr(&res);
        }
        return parse_paren_expr();
    }

    ExprPtr Parser::parse_pow_expr() { // **
        ExprPtr left = parse_var_expr();
        Lexer::Token *curr = lookahead(0);
        if(curr->type == Lexer::Type::DStar) {
            ++index;
            ExprPtr right = parse_pow_expr();
            Expr res = ExprBinop(left, *curr, right);
            return ExprPtr(&res);
        }
        return left;
    }

    ExprPtr Parser::parse_unop_expr() { // ~ -
        Lexer::Token *curr = lookahead(0);
        if(curr->type == Lexer::Type::Negate || curr->type == Lexer::Type::Minus) {
            ++index;
            ExprPtr right = parse_bit_xor_expr();
            Expr res = ExprUnop(*curr, right);
            return ExprPtr(&res);
        }
        return parse_pow_expr();
    }

    ExprPtr Parser::parse_multdiv_expr() {  //      * / % //
        ExprPtr left = parse_unop_expr();
        Lexer::Token *curr = lookahead(0);
        if(curr->type == Lexer::Type::DSlash || curr->type == Lexer::Type::Star || curr->type == Lexer::Type::Slash || curr->type == Lexer::Type::Mod) {
            ++index;
            ExprPtr right = parse_multdiv_expr();
            Expr res = ExprBinop(left, *curr, right);
            return ExprPtr(&res);
        }
        return left;
    }

    ExprPtr Parser::parse_addsub_expr() { // + -
        ExprPtr left = parse_multdiv_expr();
        Lexer::Token *curr = lookahead(0);
        if(curr->type == Lexer::Type::Plus || curr->type == Lexer::Type::Minus) {
            ++index;
            ExprPtr right = parse_bit_and_expr();
            Expr res = ExprBinop(left, *curr, right);
            return ExprPtr(&res);
        }
        return left;
    }

    ExprPtr Parser::parse_bit_shift_expr() { // << >>
        ExprPtr left = parse_addsub_expr();
        Lexer::Token *curr = lookahead(0);
        if(curr->type == Lexer::Type::ShLeft || curr->type == Lexer::Type::ShRight) {
            ++index;
            ExprPtr right = parse_bit_shift_expr();
            Expr res = ExprBinop(left, *curr, right);
            return ExprPtr(&res);
        }
        return left;
    }

    ExprPtr Parser::parse_bit_and_expr() {
        ExprPtr left = parse_bit_shift_expr();
        Lexer::Token *curr = lookahead(0);
        if(curr->type == Lexer::Type::BitAnd) {
            ++index;
            ExprPtr right = parse_bit_and_expr();
            Expr res = ExprBinop(left, *curr, right);
            return ExprPtr(&res);
        }
        return left;
    }

    ExprPtr Parser::parse_bit_xor_expr() {
        ExprPtr left = parse_bit_and_expr();
        Lexer::Token *curr = lookahead(0);
        if(curr->type == Lexer::Type::Xor) {
            ++index;
            ExprPtr right = parse_bit_xor_expr();
            Expr res = ExprBinop(left, *curr, right);
            return ExprPtr(&res);
        }
        return left;
    }

    ExprPtr Parser::parse_bit_or_expr() {
        ExprPtr left = parse_bit_xor_expr();
        Lexer::Token *curr = lookahead(0);
        if(curr->type == Lexer::Type::BitOr) {
            ++index;
            ExprPtr right = parse_bit_or_expr();
            return std::make_shared<Expr>(ExprBinop{left, *curr, right});
        }
        return left;
    }

    ExprPtr Parser::parse_cmp_expr() {
        ExprPtr left = parse_bit_or_expr();
        Lexer::Token *curr = lookahead(0);
        if(tokenCmp.contains(curr->type)) {
            
            ++index;
            ExprPtr right = parse_cmp_expr();

            ExprPtr res = std::make_shared<Expr>(ExprBinop{left, *curr, right});
            while(tokenCmp.contains(lookahead(0)->type)) {
                --index;
                left = right;
                right = parse_cmp_expr();
                ExprPtr newCond = std::make_shared<Expr>(ExprBinop{left, *curr, right});
                Lexer::Token op();
                res = std::make_shared<Expr>(ExprBinop{res, op, newCond});
            }
            return res;
        }
        return left;
    }

    ExprPtr Parser::parse_not_expr() {
        ExprPtr left = parse_cmp_expr();
        Lexer::Token *curr = lookahead(0);
        if(curr->type == Lexer::Type::Not) {
            ++index;
            ExprPtr right = parse_not_expr();
            return std::make_shared<Expr>(ExprBinop{left, *curr, right});
        }
        return left;
    }

    ExprPtr Parser::parse_and_expr() {
        ExprPtr left = parse_not_expr();
        Lexer::Token *curr = lookahead(0);
        if(curr->type == Lexer::Type::And) {
            ++index;
            ExprPtr right = parse_and_expr();
            return std::make_shared<Expr>(ExprBinop{left, *curr, right});
        }
        return left;
    }

    ExprPtr Parser::parse_or_expr() {
        ExprPtr left = parse_and_expr();
        Lexer::Token *curr = lookahead(0);
        if(curr->type == Lexer::Type::Or) {
            ++index;
            ExprPtr right = parse_or_expr();
            return std::make_shared<Expr>(ExprBinop{left, *curr, right});
        }
        return left;
    }
}   
