#pragma once

#include <iostream>
#include <string>
#include <variant>
#include <optional>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "lexer.h"

namespace Parser {
    // End Conditions:
    // ) ] } : 
    // \n if parenCount == 0
    std::unordered_set<Lexer::Type> tokenLiteral = {
        Lexer::Type::Float, Lexer::Type::Int, 
        Lexer::Type::String, Lexer::Type::None,
        Lexer::Type::True, Lexer::Type::False,
        Lexer::Type::Id
    };
    std::unordered_set<Lexer::Type> tokenCmp = {
        Lexer::Type::GreaterEqual, Lexer::Type::Greater,
        Lexer::Type::GreaterEqual, Lexer::Type::Greater,
        Lexer::Type::NotEqual, Lexer::Type::Equal
    };
    std::unordered_set<Lexer::Type> tokenAssign = {
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
    struct ExprLiteral     { Lexer::TokenPtr value; };
    struct ExprId          { Lexer::TokenPtr id; };
    struct ExprBinop       { ExprPtr left; Lexer::TokenPtr op; ExprPtr right; };
    struct ExprUnop        { Lexer::TokenPtr op; ExprPtr expr; };
    struct ExprFunc        { ExprPtr id; std::vector<ExprPtr> args; };
    struct ExprIndex       { ExprPtr id; std::vector<ExprPtr> args; };
    struct ExprList        { std::vector<ExprPtr> elements; };
    struct ExprTuple       { std::vector<ExprPtr> elements; };
    struct ExprDict        { std::vector<std::tuple<ExprPtr,ExprPtr>> pairs; };

    struct Expr : std::variant<std::monostate, ExprLiteral, ExprId, ExprBinop, 
                                ExprUnop, ExprFunc, ExprIndex, ExprList, ExprTuple, ExprDict> {
        using variant::variant;
    };

    // Statement types
    struct StmtExpression  { ExprPtr expr; };
    struct StmtAssign      { std::string name; Lexer::TokenPtr op; ExprPtr expr; };
    struct StmtBlock       { std::vector<StmtPtr> stmts; };
    // Each condition aligns with a body, if there is an extra body that is the else statement
    struct StmtIf          { std::vector<ExprPtr> conditions; std::vector<StmtPtr> bodies; };
    struct StmtFor         { ExprPtr condition; StmtPtr body; }; // the body is a block
    struct StmtWhile       { ExprPtr condition; StmtPtr body; };
    struct StmtFunc        { std::string name; std::vector<std::tuple<std::string, TypeName>> args;
                             StmtPtr body; };
    struct StmtReturn      { ExprPtr value; };
    struct StmtContinue    {};
    struct StmtBreak       {};
    struct StmtPass        {};
    struct StmtComment     { Lexer::TokenPtr comment; };
    // TODO:
    // add classes and imports

    struct Stmt : std::variant<std::monostate, StmtExpression, StmtAssign, StmtBlock, StmtIf, 
        StmtFor, StmtWhile, StmtFunc, StmtReturn, StmtContinue, StmtBreak, StmtPass, StmtComment> {
        using variant::variant;
    };

    class Parser {
    public:
        Parser(std::vector<Lexer::TokenPtr> tokens);
        std::vector<StmtPtr> *parse();
        void print();
        void print_stmt(StmtPtr stmt);
        void print_expr(ExprPtr expr);
        std::string type_to_string(TypeName type);
    private:
        size_t index = 0;
        std::vector<Lexer::TokenPtr> tokens;
        std::vector<StmtPtr> stmts;
        std::vector<size_t> scope_stack{0};

        StmtPtr parse_stmt();

        ExprPtr parse_expr();

        // TODO:
        // finish unimplemented things
        void parse_endline();
        TypeName parse_type();
        // order of operations 
        ExprPtr parse_paren_expr();
        ExprPtr parse_callget_expr(); // id() id.id id[] id[:]
        ExprPtr parse_var_expr(); // literals, id
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
        size_t get_scope();
        size_t parse_scope();
    };
}

