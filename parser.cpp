#include "parser.hpp"

namespace Parser {
    Parser::Parser(std::vector<Lexer::TokenPtr> *tokens) : tokens(tokens) { };

    Lexer::TokenPtr Parser::lookahead(size_t increment) {
        return index+increment < tokens->size() ? (*tokens)[index+increment] : tokens->back();
    }

    Lexer::TokenPtr Parser::get(size_t i) {
        return i < tokens->size() ? (*tokens)[i] : tokens->back();
    }

    int Parser::get_scope() {
        Lexer::TokenPtr curr= lookahead(0);
        if(curr->type == Lexer::Type::EndOfFile) {
            return -1;
        }
        if(std::holds_alternative<int>(curr->literal)) {
            return std::get<int>(curr->literal);
        } else {
            throw std::runtime_error("Supposed Scope does not hold int: " + curr->lexeme);
        }
    }

    std::vector<StmtPtr> *Parser::parse() {
        while(index < tokens->size()) {
            StmtPtr stmt = parse_stmt();
            if(!stmt) {
                std::cout << "monostate found" << std::endl;
                break;
            } else { 
                // print_stmt(stmt);
                stmts.push_back(stmt); 
            }
        }
        return &stmts;
    }

    StmtPtr Parser::parse_stmt() {
        // Curr should always be a Scope token to keep track of scope
        if (lookahead(0)->type == Lexer::Type::Comment) {
            return parse_comment_stmt();
        }
        Lexer::TokenPtr scope = lookahead(0);
        if (lookahead()->lexeme == "EOF" || scope->lexeme == "EOF") {
            return nullptr;
        }
        if (scope->type != Lexer::Type::Scope) {
            throw std::runtime_error("Not a Scope token at index: " + std::to_string(index) + " " + lookahead(0)->lexeme);
        }
        if (std::get<int>(scope->literal) > scope_stack.back()) {
            throw std::runtime_error("Scope syntax is wrong at index: " + std::to_string(index));
        }
        index++;
        Lexer::TokenPtr curr = lookahead(0);
        if (lookahead(0)->type == Lexer::Type::Comment) {
            return parse_comment_stmt();
        }
        if(curr->type == Lexer::Type::Def) { // def id(args, args):\nStmtBlock || def id() -> type:
            return parse_func_stmt();
        } else if(curr->type == Lexer::Type::Id) {
            return parse_id_stmt();
        } else if(curr->type == Lexer::Type::If) {
            return parse_if_stmt();
        } else if(curr->type == Lexer::Type::For) {
            return parse_for_stmt();
        } else if(curr->type == Lexer::Type::While) {
            return parse_while_stmt();
        } else if(curr->type == Lexer::Type::Return) {
            return parse_return_stmt();
        } else if(curr->type == Lexer::Type::Continue) {
            return parse_continue_stmt();
        } else if(curr->type == Lexer::Type::Break) {
            return parse_break_stmt();
        } else if(curr->type == Lexer::Type::Pass) {
            return parse_pass_stmt();
        } else {
            ++index;
            return nullptr; // EOF or error
        }
    }

    StmtPtr Parser::parse_comment_stmt() {
        Lexer::TokenPtr curr = lookahead(0);
        // if(curr->type == Lexer::Type::Comment) {
        ++index;
        return std::make_shared<Stmt>(StmtComment(curr));
        // }
    }

    StmtPtr Parser::parse_func_stmt() {
        // def id(arg: int, arg)
        //        ^
        StmtFunc res;
        res.name = lookahead()->lexeme;
        if(lookahead()->type == Lexer::Type::Id && lookahead(2)->type == Lexer::Type::LPar) {
            index += 3;
            while(lookahead(0)->type != Lexer::Type::RPar) {
                if(lookahead(0)->type == Lexer::Type::Comma) {
                    ++index;
                } else {
                    ExprPtr arg = parse_expr();
                    std::string arg_name = std::get<ExprId>(*arg).id->lexeme;
                    TypeNamePtr type;
                    if(lookahead(0)->type == Lexer::Type::Colon) {
                        ++index;
                        type = parse_type();
                    }
                    res.args.push_back({arg_name, type});
                }
            }
            ++index;
            // Next token must be arrow or colon
            if (lookahead(0)->type == Lexer::Type::Arrow) {
                ++index;
                res.return_type = parse_type();
                // should be at colon
            }
            // Index should be at colon
            ++index;
            // Index should be at Scope
            StmtBlock block;
            if (lookahead(0)->type != Lexer::Type::Scope ) {
                if (lookahead(0)->type == Lexer::Type::Id) {
                    block.stmts.push_back(parse_id_stmt());
                } else {
                    throw std::runtime_error("Invalid Syntax for: inline conditional at index: " + std::to_string(index) + " " + lookahead(0)->lexeme);
                }
            } else {
                int new_scope = get_scope();
                if (new_scope <= scope_stack.back()) {
                    throw std::runtime_error("Invalid Syntax for: While");
                }
                scope_stack.push_back(new_scope);
                while (get_scope() != -1 && get_scope() == new_scope) {
                    block.stmts.push_back(parse_stmt());
                }
                scope_stack.pop_back();
            }
            res.body = std::make_shared<Stmt>(block);
        } else {
            throw std::runtime_error("Invalid Syntax for: def"); // print line as well
        }
        return std::make_shared<Stmt>(res);
    }

    StmtPtr Parser::parse_id_stmt() {
        Stmt res;
        Lexer::TokenPtr curr = lookahead(0);
        if(curr->type == Lexer::Type::Id) {
            ExprPtr first = parse_expr();
            curr = lookahead(0);
            if(curr->type == Lexer::Type::Assign) { // change to tokenAssign.contains() later
                // std::string id = lookahead(0)->lexeme;
                ++index; // =
                res = StmtAssign(first, curr, parse_expr());
            } else { // foo()
                res = StmtExpression(first);
            }
        } else {
            throw std::runtime_error("Invalid stmt Syntax for: Id " + curr->lexeme);
        }
        return std::make_shared<Stmt>(res);
    }

    StmtPtr Parser::parse_if_stmt() {
        Stmt res;
        ++index;
        StmtIf if_else_ladder;
        auto [condition, block] = parse_block_stmt();
        if_else_ladder.conditions.push_back(condition);
        if_else_ladder.bodies.push_back(block);

        while (lookahead()->type == Lexer::Type::Elif) {
            index += 2;
            auto [condition, block] = parse_block_stmt();
            if_else_ladder.conditions.push_back(condition);
            if_else_ladder.bodies.push_back(block);
        }
        if (lookahead()->type == Lexer::Type::Else) {
            index += 3;
            StmtBlock block;
            if (lookahead(0)->type != Lexer::Type::Scope ) {
                if (lookahead(0)->type == Lexer::Type::Id) {
                    block.stmts.push_back(parse_id_stmt());
                } else {
                    throw std::runtime_error("Invalid Syntax for: inline conditional at index: " + std::to_string(index) + " " + lookahead(0)->lexeme);
                }
            } else {
                int new_scope = get_scope();
                if (new_scope <= scope_stack.back()) {
                    throw std::runtime_error("Invalid Syntax for: While");
                }
                scope_stack.push_back(new_scope);
                while (get_scope() != -1 && get_scope() == new_scope) {
                    block.stmts.push_back(parse_stmt());
                }
                scope_stack.pop_back();
            }
            if_else_ladder.bodies.push_back(std::make_shared<Stmt>(block));
        }
        res = if_else_ladder;
        return std::make_shared<Stmt>(res);
    }

    StmtPtr Parser::parse_for_stmt() {
        Stmt res;
        ++index;
        parsing_for_loop = true;
        std::vector<ExprPtr> targets = {parse_expr()};
        while(lookahead(0)->type == Lexer::Type::Comma) {
            ++index;
            targets.push_back(parse_expr());
        }
        parsing_for_loop = false;
        // Ends at 'in' token
        ++index;
        ExprPtr iterable = parse_expr();
        if(!iterable) {
            throw std::runtime_error("Iterable expected at " + std::to_string(index) + " " + lookahead(0)->lexeme);
        }
        // Ends at ':' token
        ++index;
        // Two choices are scope token or no scope token
        StmtBlock block;
        if (lookahead(0)->type != Lexer::Type::Scope ) {
            if (lookahead(0)->type == Lexer::Type::Id) {
                block.stmts.push_back(parse_id_stmt());
            } else {
                throw std::runtime_error("Invalid Syntax for: inline conditional at index: " + std::to_string(index) + " " + lookahead(0)->lexeme);
            }
        } else {
            int new_scope = get_scope();
            if (new_scope <= scope_stack.back()) {
                throw std::runtime_error("Invalid Syntax for: While");
            }
            scope_stack.push_back(new_scope);
            while (get_scope() != -1 && get_scope() == new_scope) {
                block.stmts.push_back(parse_stmt());
            }
            scope_stack.pop_back();
        }

        res = StmtFor(targets, iterable, std::make_shared<Stmt>(block));
        return std::make_shared<Stmt>(res);
    }

    StmtPtr Parser::parse_while_stmt() {
        Stmt res;
        ++index;
        auto [condition, block] = parse_block_stmt();
        res = StmtWhile(condition, block);
        return std::make_shared<Stmt>(res);
    }

    StmtPtr Parser::parse_return_stmt() {
        Stmt res;
        ++index;
        res = StmtReturn(parse_expr());
        return std::make_shared<Stmt>(res);
    }

    StmtPtr Parser::parse_continue_stmt() {
        Stmt res;
        ++index;
        res = StmtContinue();
        return std::make_shared<Stmt>(res);
    }

    StmtPtr Parser::parse_break_stmt() {
        Stmt res;
        ++index;
        res = StmtBreak();
        return std::make_shared<Stmt>(res);
    }
    
    StmtPtr Parser::parse_pass_stmt() {
        Stmt res;
        ++index;
        res = StmtPass();
        return std::make_shared<Stmt>(res);
    }

    // Index should be at the conditional
    std::tuple<ExprPtr, StmtPtr> Parser::parse_block_stmt() {
        ExprPtr condition = parse_expr();
        // Ends at : token
        ++index;
        // Two choices are scope token or no scope token
        StmtBlock block;
        if (lookahead(0)->type != Lexer::Type::Scope ) {
            if (lookahead(0)->type == Lexer::Type::Id) {
                block.stmts.push_back(parse_id_stmt());
            } else {
                throw std::runtime_error("Invalid Syntax for: inline conditional at index: " + std::to_string(index) + " " + lookahead(0)->lexeme);
            }
        } else {
            int new_scope = get_scope();
            if (new_scope <= scope_stack.back()) {
                throw std::runtime_error("Invalid Syntax for: conditional");
            }
            scope_stack.push_back(new_scope);
            while (get_scope() != -1 && get_scope() == new_scope) {
                block.stmts.push_back(parse_stmt());
            }
            scope_stack.pop_back();
        }
        return {condition, std::make_shared<Stmt>(block)};
    }



    void Parser::print() {
        for(auto& stmt: stmts) {
            if(stmt) {
                print_stmt(stmt);
            } else {
                std::cout << "Stmt is nullptr" << std::endl;
            }
        }
    }

    void Parser::print_stmt(StmtPtr stmt) { // make sure to implement comments here somewhere
        if(!stmt) {
            throw std::runtime_error("print_stmt: stmt == nullptr");
        }
        if (std::holds_alternative<StmtExpression>(*stmt)) {
            auto& stmtExpression = std::get<StmtExpression>(*stmt);
            std::cout << "StmtExpression" << std::endl;
            print_expr(stmtExpression.expr);
        } else if (std::holds_alternative<StmtAssign>(*stmt)) {
            auto& stmtAssign = std::get<StmtAssign>(*stmt);
            std::cout << "StmtAssign" << std::endl;
            print_expr(stmtAssign.name);
            std::cout << " " << stmtAssign.op->lexeme << std::endl;
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
            for(auto e : stmtFor.targets) {
                print_expr(e);
            }
            print_expr(stmtFor.iterable);
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
                std::cout << type_to_string(std::get<1>(stmtFunc.args[i])) << " " << std::get<0>(stmtFunc.args[i]) << std::endl;
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
        } else if (std::holds_alternative<StmtComment>(*stmt)) {
            std::cout << "StmtComment: " << std::get<StmtComment>(*stmt).comment->lexeme << std::endl;
        } else {
            std::cout << "Stmt monostate" << std::endl;
        }
    }

    std::string type_to_string(TypeNamePtr type) {
        if(type->type == Int) { return "int"; }
        else if(type->type == Float) { return "float"; }
        else if(type->type == Bool) { return "bool"; }
        else if(type->type == NoneType) { return "NoneType"; }
        else if(type->type == List) { 
            std::string res = "list";
            res += "[";
            res += type_to_string(type->inside);
            res += "]";
            return res;
        }
        else if(type->type == Dict) { return "dict"; }
        else if(type->type == Tuple) { return "tuple"; }
        else if(type->type == Set) { return "set"; }
        else if(type->type == Range) { return "range"; }
        else if(type->type == Enumerate) { return "enumerate"; }
        else if(type->type == Zip) { return "zip"; }
        else if(type->type == Slice) { return "slice"; }
        else if(type->type == Frozenset) { return "frozenset"; }
        else if(type->type == Function) { return "function"; }
        else if(type->type == Lambda) { return "lambda"; }
        else if(type->type == Type) { return "type"; }
        else if(type->type == Any) { return "Any"; }
        else { return "you missed something bro"; }
    }

    // =-=-=-=-=-=-=-=-=-= =-=-=-=-=-=-=-=-=-= Parse Expression =-=-=-=-=-=-=-=-=-= =-=-=-=-=-=-=-=-=-=

    ExprPtr Parser::parse_expr() {
        // std::cout << "Parsing expr at " << index << " " << lookahead(0)->lexeme << std::endl;
        return parse_in_expr();
    }

    void Parser::parse_endline() {
        Lexer::TokenPtr curr = lookahead(0);
        while(curr->type != Lexer::Type::Scope) {
            if(curr->type == Lexer::Type::Comment) {
                stmts.push_back(std::make_shared<Stmt>(StmtComment(curr))); 
            }
            ++index;
        }
    }

    TypeNamePtr Parser::parse_type() {
        Lexer::TokenPtr curr = lookahead(0);
        Lexer::Literal type = curr->literal;
        TypeName res;
        if(std::holds_alternative<std::string>(type)) {
            ++index;
            std::string type_name = std::get<std::string>(type);
            if(type_name == "int") { res.type = TypeNameEnum::Int; }
            else if(type_name == "float") { res.type = TypeNameEnum::Float; }
            else if(type_name == "bool") { res.type = TypeNameEnum::Bool; }
            else if(type_name == "NoneType") { res.type = TypeNameEnum::NoneType; }
            else if(type_name == "list") {
                res.type = TypeNameEnum::List;
                ++index; // [ 
                res.inside = parse_type();
                ++index; // ]
            }
            else if(type_name == "dict") { res.type = TypeNameEnum::Dict; }
            else if(type_name == "tuple") { res.type = TypeNameEnum::Tuple; }
            else if(type_name == "set") { res.type = TypeNameEnum::Set; }
            else if(type_name == "range") { res.type = TypeNameEnum::Range; }
            else if(type_name == "enumerate") { res.type = TypeNameEnum::Enumerate; }
            else if(type_name == "zip") { res.type = TypeNameEnum::Zip; }
            else if(type_name == "slice") { res.type = TypeNameEnum::Slice; }
            else if(type_name == "frozenset") { res.type = TypeNameEnum::Frozenset; }
            else if(type_name == "function") { res.type = TypeNameEnum::Function; }
            else if(type_name == "lambda") { res.type = TypeNameEnum::Lambda; }
            else if(type_name == "type") { res.type = TypeNameEnum::Type; }
        } else {
            throw std::runtime_error("Expected Typename at line " + std::to_string(curr->line) + ": " + curr->lexeme);
        }
        return std::make_shared<TypeName>(res);
    }

    ExprPtr Parser::parse_paren_expr() {
        Lexer::TokenPtr curr = lookahead(0);
        if(curr->type == Lexer::Type::LPar) { // tuple or paren (expr) or (expr,expr) or (expr,)
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
            return std::make_shared<Expr>(res);
        } else if(curr->type == Lexer::Type::LSquare) { // list [expr,expr] or [expr,] (allows trailing comma)
            Expr res = ExprList();
            std::vector<ExprPtr> *elements = &std::get<ExprList>(res).elements;
            ++index;
            while(lookahead(0)->type != Lexer::Type::RSquare) {
                if(lookahead(0)->type == Lexer::Type::Comma) {
                    ++index;
                } else {
                    ExprPtr expr = parse_expr();
                    if(expr) { elements->push_back(expr); }
                }
            }
            ++index;
            return std::make_shared<Expr>(res);
        } else if(curr->type == Lexer::Type::LBrace) { // dict {expr: expr, expr: expr} (doesn't allow trailing comma)
            ExprDict res = ExprDict();
            std::vector<std::tuple<ExprPtr,ExprPtr>> *pairs = &res.pairs;
            ++index;
            while(lookahead(0)->type != Lexer::Type::RBrace) {
                if(lookahead(0)->type == Lexer::Type::Comma) {
                    ++index;
                } else { 
                    ExprPtr key = parse_expr();
                    ++index;
                    if(key) {
                        ++index; // :
                        ExprPtr value = parse_expr();
                        pairs->push_back(std::tuple<ExprPtr,ExprPtr>(key,value));
                    }
                }
            }
            ++index;
            return std::make_shared<Expr>(res);
        }
        std::cout << "returning nullptr at" << std::endl;
        print_curr();
        return nullptr;
    }

    ExprPtr Parser::parse_id_expr() { // index is currently at an id
        ExprPtr id = std::make_shared<Expr>(ExprId(lookahead(0)));
        ++index;
        Lexer::TokenPtr curr = lookahead(0);
        if(curr->type == Lexer::Type::LPar) { // function call
            ExprFunc res = ExprFunc();
            res.id = id;
            // get and set args
            ++index; // (
            while(lookahead(0)->type != Lexer::Type::RPar) {
                if(lookahead(0)->type == Lexer::Type::Comma) {
                    ++index;
                } else {
                    ExprPtr arg = parse_expr();
                    res.args.push_back(arg);
                }
            }
            ++index; // )
            return std::make_shared<Expr>(res);
        } else if(curr->type == Lexer::Type::LSquare) { // []
            ExprIndex res;
            res.id = id;
            ++index;
            while(lookahead(0)->type != Lexer::Type::RSquare) {
                if(lookahead(0)->type == Lexer::Type::Comma) { ++index; }
                else {
                    // get is safe here because res is always an ExprIndex
                    res.args.push_back(parse_expr());
                }
            }
            ++index;
            return std::make_shared<Expr>(res);
        } else if(curr->type == Lexer::Type::Period) { // .
            index++; // .
            ExprPtr right = parse_id_expr();
            return std::make_shared<Expr>(ExprBinop(id, curr, right));
        }
        return id;
    }

    ExprPtr Parser::parse_var_expr() { // id int bool float str
        Lexer::TokenPtr curr = lookahead(0);
        if(curr->type == Lexer::Type::Id) {
            return parse_id_expr();
        } else if(tokenLiteral.contains(curr->type)) {
            ++index;
            return std::make_shared<Expr>(ExprLiteral(curr));
        }
        return parse_paren_expr();
    }

    ExprPtr Parser::parse_pow_expr() { // **
        ExprPtr left = parse_var_expr();
        Lexer::TokenPtr curr = lookahead(0);
        if(curr->type == Lexer::Type::DStar) {
            ++index;
            ExprPtr right = parse_pow_expr();
            return std::make_shared<Expr>(ExprBinop{left, curr, right});
        }
        return left;
    }

    ExprPtr Parser::parse_unop_expr() { // ~ -
        Lexer::TokenPtr curr = lookahead(0);
        if(curr->type == Lexer::Type::Negate || curr->type == Lexer::Type::Minus) {
            ++index;
            ExprPtr right = parse_bit_xor_expr();
            return std::make_shared<Expr>(ExprUnop{curr, right});
        }
        return parse_pow_expr();
    }

    ExprPtr Parser::parse_multdiv_expr() {  //      * / % //
        ExprPtr left = parse_unop_expr();
        Lexer::TokenPtr curr = lookahead(0);
        if(curr->type == Lexer::Type::DSlash || curr->type == Lexer::Type::Star || curr->type == Lexer::Type::Slash || curr->type == Lexer::Type::Mod) {
            ++index;
            ExprPtr right = parse_multdiv_expr();
            return std::make_shared<Expr>(ExprBinop{left, curr, right});
        }
        return left;
    }

    ExprPtr Parser::parse_addsub_expr() { // + -
        ExprPtr left = parse_multdiv_expr();
        Lexer::TokenPtr curr = lookahead(0);
        if(curr->type == Lexer::Type::Plus || curr->type == Lexer::Type::Minus) {
            ++index;
            ExprPtr right = parse_addsub_expr();
            return std::make_shared<Expr>(ExprBinop{left, curr, right});
        }
        return left;
    }

    ExprPtr Parser::parse_bit_shift_expr() { // << >>
        ExprPtr left = parse_addsub_expr();
        Lexer::TokenPtr curr = lookahead(0);
        if(curr->type == Lexer::Type::ShLeft || curr->type == Lexer::Type::ShRight) {
            ++index;
            ExprPtr right = parse_bit_shift_expr();
            return std::make_shared<Expr>(ExprBinop{left, curr, right});
        }
        return left;
    }

    ExprPtr Parser::parse_bit_and_expr() {
        ExprPtr left = parse_bit_shift_expr();
        Lexer::TokenPtr curr = lookahead(0);
        if(curr->type == Lexer::Type::BitAnd) {
            ++index;
            ExprPtr right = parse_bit_and_expr();
            return std::make_shared<Expr>(ExprBinop{left, curr, right});
        }
        return left;
    }

    ExprPtr Parser::parse_bit_xor_expr() {
        ExprPtr left = parse_bit_and_expr();
        Lexer::TokenPtr curr = lookahead(0);
        if(curr->type == Lexer::Type::Xor) {
            ++index;
            ExprPtr right = parse_bit_xor_expr();
            return std::make_shared<Expr>(ExprBinop{left, curr, right});
        }
        return left;
    }

    ExprPtr Parser::parse_bit_or_expr() {
        ExprPtr left = parse_bit_xor_expr();
        Lexer::TokenPtr curr = lookahead(0);
        if(curr->type == Lexer::Type::BitOr) {
            ++index;
            ExprPtr right = parse_bit_or_expr();
            return std::make_shared<Expr>(ExprBinop{left, curr, right});
        }
        return left;
    }

    ExprPtr Parser::parse_cmp_expr() {
        ExprPtr left = parse_bit_or_expr();
        Lexer::TokenPtr curr = lookahead(0);
        if(tokenCmp.contains(curr->type)) {
            Lexer::TokenPtr op = curr;
            // a < b < c < d < e
            //     l o r ^ 
            ++index;
            ExprPtr right = parse_bit_or_expr();

            ExprPtr res = std::make_shared<Expr>(ExprBinop{left, op, right});
            while(tokenCmp.contains(lookahead(0)->type)) {
                left = right;
                op = lookahead(0);
                ++index;
                // right = parse_cmp_expr();
                right = parse_bit_or_expr();
                ExprPtr newCond = std::make_shared<Expr>(ExprBinop{left, op, right});
                Lexer::TokenPtr and_op = std::make_shared<Lexer::Token>(Lexer::Token(Lexer::Type::And));
                res = std::make_shared<Expr>(ExprBinop{res, and_op, newCond});
            }
            return res;
        }
        return left;
    }

    ExprPtr Parser::parse_not_expr() {
        ExprPtr left = parse_cmp_expr();
        Lexer::TokenPtr curr = lookahead(0);
        if(curr->type == Lexer::Type::Not) {
            ++index;
            ExprPtr right = parse_not_expr();
            return std::make_shared<Expr>(ExprBinop{left, curr, right});
        }
        return left;
    }

    ExprPtr Parser::parse_and_expr() {
        ExprPtr left = parse_not_expr();
        Lexer::TokenPtr curr = lookahead(0);
        if(curr->type == Lexer::Type::And) {
            ++index;
            ExprPtr right = parse_and_expr();
            return std::make_shared<Expr>(ExprBinop{left, curr, right});
        }
        return left;
    }

    ExprPtr Parser::parse_or_expr() {
        ExprPtr left = parse_and_expr();
        Lexer::TokenPtr curr = lookahead(0);
        if(curr->type == Lexer::Type::Or) {
            ++index;
            ExprPtr right = parse_or_expr();
            return std::make_shared<Expr>(ExprBinop{left, curr, right});
        }
        return left;
    }

    ExprPtr Parser::parse_in_expr() {
        ExprPtr left = parse_or_expr();
        Lexer::TokenPtr curr = lookahead(0);
        if(!parsing_for_loop && curr->type == Lexer::Type::In) {
            Lexer::TokenPtr op = curr;
            // a < b < c < d < e
            //     l o r ^ 
            ++index;
            ExprPtr right = parse_in_expr();

            ExprPtr res = std::make_shared<Expr>(ExprBinop{left, op, right});
            while(tokenCmp.contains(lookahead(0)->type)) {
                left = right;
                op = lookahead(0);
                ++index;
                right = parse_in_expr();
                ExprPtr newCond = std::make_shared<Expr>(ExprBinop{left, op, right});
                Lexer::TokenPtr and_op = std::make_shared<Lexer::Token>(Lexer::Token(Lexer::Type::And));
                res = std::make_shared<Expr>(ExprBinop{res, and_op, newCond});
            }
            return res;
        }
        return left;
    }

    // print_expr ends in a newline
    void Parser::print_expr(ExprPtr expr) {
        if(!expr) {
            std::cout << "print_expr: expr == nullptr" << std::endl;
        }
        if (std::holds_alternative<ExprLiteral>(*expr)) {
            auto& exprLiteral = std::get<ExprLiteral>(*expr);
            std::cout << "ExprLiteral" << std::endl; 
            std::cout << exprLiteral.value->lexeme << std::endl;
        } else if (std::holds_alternative<ExprId>(*expr)) {
            auto& exprId = std::get<ExprId>(*expr);
            std::cout << "ExprId" << std::endl; 
            std::cout << exprId.id->lexeme << std::endl;
        } else if (std::holds_alternative<ExprBinop>(*expr)) {
            auto& exprBinop = std::get<ExprBinop>(*expr);
            std::cout << "ExprBinop" << std::endl; 
            print_expr(exprBinop.left);
            std::cout << exprBinop.op->lexeme << std::endl;
            print_expr(exprBinop.right);
        } else if (std::holds_alternative<ExprUnop>(*expr)) {
            auto& exprUnop = std::get<ExprUnop>(*expr);
            std::cout << "ExprUnop" << std::endl; 
            std::cout << exprUnop.op->lexeme << std::endl;
            print_expr(exprUnop.expr);
        } else if (std::holds_alternative<ExprFunc>(*expr)) {
            auto& exprFunc = std::get<ExprFunc>(*expr);
            std::cout << "ExprFunc" << std::endl; 
            print_expr(exprFunc.id);
            for (size_t i = 0; i < exprFunc.args.size(); ++i) {
                print_expr(exprFunc.args[i]);
            }
        } else if (std::holds_alternative<ExprIndex>(*expr)) {
            auto& exprIndex = std::get<ExprIndex>(*expr);
            std::cout << "ExprIndex" << std::endl; 
            print_expr(exprIndex.id);
            for (size_t i = 0; i < exprIndex.args.size(); ++i) {
                print_expr(exprIndex.args[i]);
            }
        } else if (std::holds_alternative<ExprList>(*expr)) {
            auto& exprList = std::get<ExprList>(*expr);
            std::cout << "ExprList" << std::endl; 
            for (size_t i = 0; i < exprList.elements.size(); ++i) {
                print_expr(exprList.elements[i]);
            }
        } else if (std::holds_alternative<ExprTuple>(*expr)) {
            auto& exprTuple = std::get<ExprTuple>(*expr);
            std::cout << "ExprTuple" << std::endl; 
            for (size_t i = 0; i < exprTuple.elements.size(); ++i) {
                print_expr(exprTuple.elements[i]);
            }
        } else if (std::holds_alternative<ExprDict>(*expr)) {
            auto& exprDict = std::get<ExprDict>(*expr);
            std::cout << "ExprDict" << std::endl; 
            for (size_t i = 0; i < exprDict.pairs.size(); ++i) {
                print_expr(std::get<0>(exprDict.pairs[i]));
                print_expr(std::get<1>(exprDict.pairs[i]));
            }
        }
    }

    void Parser::print_curr() {
        std::cout << "Current Token: " << index << " " << lookahead(0)->lexeme << std::endl;
    }
}   
