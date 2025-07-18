#pragma once

#include <iostream>
#include <string>
#include <variant>
#include <memory>
#include <optional>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "lexer.hpp"

namespace Parser {
    // End Conditions:
    // ) ] } : 
    // \n if parenCount == 0
    inline std::unordered_set<Lexer::Type> tokenLiteral = {
        Lexer::Type::Float, Lexer::Type::Int, 
        Lexer::Type::String, Lexer::Type::None,
        Lexer::Type::True, Lexer::Type::False,
        Lexer::Type::Id
    };
    inline std::unordered_set<Lexer::Type> tokenCmp = {
        Lexer::Type::GreaterEqual, Lexer::Type::Greater,
        Lexer::Type::LessEqual, Lexer::Type::Less,
        Lexer::Type::NotEqual, Lexer::Type::Equal
    };
    inline std::unordered_set<Lexer::Type> tokenAssign = {
        // = *= /= += -= %=
        Lexer::Type::Assign
    };
    // std::unordered_set<Lexer::Type> tokenEndExpr = {
    //     Lexer::Type::Colon, Lexer::Type::Newline,
    //     Lexer::Type::Comma
    // };

    struct Expr;
    struct Stmt;

    enum TypeName { 
        Int, Float, Bool, NoneType,
        
        List, Dict, Tuple, Set, Range, Enumerate, Zip, Slice, Frozenset,

        Function, Lambda, Type,

        Any
    };

    struct Argument { std::string id; std::string type; };

    using ExprPtr = std::shared_ptr<Expr>;
    using StmtPtr = std::shared_ptr<Stmt>;

    // Expression types
    struct ExprLiteral { 
        Lexer::TokenPtr value; 
        ExprLiteral(Lexer::TokenPtr value) : value(value) {};
        ExprLiteral() {}; 
    };
    struct ExprId { 
        Lexer::TokenPtr id; 
        ExprId(Lexer::TokenPtr id) : id(id) {}; 
        ExprId() {}; 
    };
    struct ExprBinop { 
        ExprPtr left; Lexer::TokenPtr op; ExprPtr right;
        ExprBinop(ExprPtr left, Lexer::TokenPtr op, ExprPtr right) : left(left), op(op), right(right) {}; 
        ExprBinop() {}; 
    };
    struct ExprUnop {
        Lexer::TokenPtr op; ExprPtr expr; 
        ExprUnop(Lexer::TokenPtr op, ExprPtr expr) : op(op), expr(expr) {}; 
        ExprUnop() {}; 
    };
    struct ExprFunc {
        ExprPtr id; std::vector<ExprPtr> args; 
        ExprFunc(ExprPtr id, std::vector<ExprPtr> args) : id(id), args(args) {}; 
        ExprFunc() {}; 
    };
    struct ExprIndex {
        ExprPtr id; std::vector<ExprPtr> args; 
        ExprIndex(ExprPtr id, std::vector<ExprPtr> args) : id(id), args(args) {}; 
        ExprIndex() {};
    };
    struct ExprList {
        std::vector<ExprPtr> elements; 
        ExprList(std::vector<ExprPtr> elements) : elements(elements) {}; 
        ExprList() {};
    };
    struct ExprTuple {
        std::vector<ExprPtr> elements; 
        ExprTuple(std::vector<ExprPtr> elements) : elements(elements) {}; 
        ExprTuple() {}; 
    };
    struct ExprDict {
        std::vector<std::tuple<ExprPtr,ExprPtr>> pairs; 
        ExprDict(std::vector<std::tuple<ExprPtr,ExprPtr>> pairs) : pairs(pairs) {}; 
        ExprDict() {}; 
    };
    struct Expr : std::variant<std::monostate, ExprLiteral, ExprId, ExprBinop, 
                                ExprUnop, ExprFunc, ExprIndex, ExprList, ExprTuple, ExprDict> {
        using variant::variant;
    };

    // Statement types
    struct StmtExpression { ExprPtr expr;
        StmtExpression(ExprPtr expr) : expr(expr) {}; 
        StmtExpression() {}; 
    };
    struct StmtAssign {
        std::string name; Lexer::TokenPtr op; ExprPtr expr; 
        StmtAssign(std::string name, Lexer::TokenPtr op, ExprPtr expr) : name(name), op(op), expr(expr) {}; 
        StmtAssign() {};
    };
    struct StmtBlock {
        std::vector<StmtPtr> stmts; 
        StmtBlock(std::vector<StmtPtr> stmts) : stmts(stmts) {}; 
        StmtBlock() {};
    };
    // Each condition aligns with a body, if there is an extra body that is the else statement
    struct StmtIf {
        std::vector<ExprPtr> conditions; std::vector<StmtPtr> bodies; 
        StmtIf(std::vector<ExprPtr> conditions, std::vector<StmtPtr> bodies) : conditions(conditions), bodies(bodies) {}; 
        StmtIf() {}; 
    };
    struct StmtFor {
        ExprPtr target; ExprPtr iterable; StmtPtr body; 
        StmtFor(ExprPtr target, ExprPtr iterable, StmtPtr body) : target(target), iterable(iterable), body(body) {}; 
        StmtFor() {}; 
    }; // the body is a block
    struct StmtWhile {
        ExprPtr condition; StmtPtr body; 
        StmtWhile(ExprPtr condition, StmtPtr body) : condition(condition), body(body) {}; 
        StmtWhile() {}; 
    };
    struct StmtFunc {
        std::string name; std::vector<std::tuple<std::string, TypeName>> args; TypeName return_type; StmtPtr body; 
        StmtFunc(std::string name, std::vector<std::tuple<std::string, TypeName>> args, TypeName return_type, 
            StmtPtr body) : name(name), args(args), return_type(return_type), body(body) {}; 
        StmtFunc() {}; 
    };
    struct StmtReturn {
        ExprPtr value; 
        StmtReturn(ExprPtr value) : value(value) {};
    };
    struct StmtContinue { StmtContinue() {}; };
    struct StmtBreak { StmtBreak() {}; };
    struct StmtPass { StmtPass() {}; };
    struct StmtComment {
        Lexer::TokenPtr comment; 
        StmtComment(Lexer::TokenPtr comment) : comment(comment) {}; 
        StmtComment() {};
    };
    // TODO:
    // add classes and imports

    struct Stmt : std::variant<std::monostate, StmtExpression, StmtAssign, StmtBlock, StmtIf, 
        StmtFor, StmtWhile, StmtFunc, StmtReturn, StmtContinue, StmtBreak, StmtPass, StmtComment> {
        using variant::variant;
    };

    class Parser {
    public:
        Parser(std::vector<Lexer::TokenPtr> *tokens);
        std::vector<StmtPtr> *parse();
        void print();
        void print_stmt(StmtPtr stmt);
        void print_expr(ExprPtr expr);
        std::string type_to_string(TypeName type);

        // TEMP
        auto get_tokens() { return tokens; };
    private:
        size_t index = 0;
        std::vector<Lexer::TokenPtr> *tokens;
        std::vector<StmtPtr> stmts;
        std::vector<int> scope_stack{0};

        StmtPtr parse_stmt();
        std::tuple<ExprPtr, StmtPtr> parse_block_stmt();
        StmtPtr parse_func_stmt();
        StmtPtr parse_id_stmt();
        StmtPtr parse_if_stmt();
        StmtPtr parse_for_stmt();
        StmtPtr parse_while_stmt();
        StmtPtr parse_return_stmt();
        StmtPtr parse_continue_stmt();
        StmtPtr parse_break_stmt();
        StmtPtr parse_pass_stmt();

        ExprPtr parse_expr();
        // TODO:
        // finish unimplemented things
        void parse_endline();
        TypeName parse_type();
        // recursive descent parser
        ExprPtr parse_paren_expr();
        ExprPtr parse_id(); // id() id.id id[] id[:] id
        ExprPtr parse_var_expr(); // literals
        ExprPtr parse_pow_expr(); // **
        ExprPtr parse_unop_expr(); // ~ -
        ExprPtr parse_multdiv_expr(); // * // / %
        ExprPtr parse_addsub_expr(); // + -
        ExprPtr parse_bit_shift_expr(); // << >>
        ExprPtr parse_bit_and_expr(); // &
        ExprPtr parse_bit_xor_expr(); // ^
        ExprPtr parse_bit_or_expr(); // |
        ExprPtr parse_cmp_expr(); // != == <= < >= >
        //ExprPtr parse_assign_expr(); // :=
        ExprPtr parse_not_expr(); // not
        ExprPtr parse_and_expr(); // and
        ExprPtr parse_or_expr(); // or
        // ExprPtr parse_ternary_expr(); // _ if _ else _
        // ExprPtr parse_lambda_expr(); // lambda arg1, arg2: expr

        // returns the next TokenPtr or EndOfFile if end is reached
        Lexer::TokenPtr lookahead(size_t increment = 1);
        Lexer::TokenPtr get(size_t i);

        int get_scope();
    };
}

