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

    std::unordered_set<Lexer::Type> tokenLParen = {
        Lexer::Type::LPar, Lexer::Type::LSquare, Lexer::Type::LBrace
    };
    std::unordered_set<Lexer::Type> tokenRParen = {
        Lexer::Type::RPar, Lexer::Type::RSquare, Lexer::Type::RBrace
    };
    std::unordered_set<Lexer::Type> tokenLiteral = {
        Lexer::Type::Float, Lexer::Type::Int, 
        Lexer::Type::String, Lexer::Type::None,
        Lexer::Type::True, Lexer::Type::False,
        Id
    };
    std::unordered_set<Lexer::Type> tokenBinop = {
        Lexer::Type::DSlash, Lexer::Type::Slash, 
        Lexer::Type::Plus, Lexer::Type::Minus, 
        Lexer::Type::Star, Lexer::Type::Period,
        Lexer::Type::And, Lexer::Type::Or,
        Lexer::Type::BitOr, Lexer::Type::BitAnd, 
        Lexer::Type::ShRight, Lexer::Type::ShLeft, 
        Lexer::Type::Xor, Lexer::Type::In,
        Lexer::Type::Is
    };
    std::unordered_set<Lexer::Type> tokenUnop = {
        Lexer::Type::Negate, Lexer::Type::Minus
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

        Function, Lambda, Type
    };

    struct Argument { std::string id; std::string type; };

    using ExprPtr = std::shared_ptr<Expr>;
    using StmtPtr = std::shared_ptr<Stmt>;

    // Expression types
    struct ExprLiteral     { Lexer::Literal value; };
    struct ExprId          { Lexer::Token id; };
    struct ExprBinop       { ExprPtr left; Lexer::Token op; ExprPtr right; };
    struct ExprUnop        { Lexer::Token op; ExprPtr expr; };
    struct ExprFunc        { ExprId id; std::vector<ExprPtr> args; };
    struct ExprIndex       { ExprId id; std::vector<ExprPtr> args; };
    struct ExprList        { std::vector<ExprPtr> elements; };
    struct ExprDict        { std::vector<std::tuple<ExprPtr,ExprPtr>> pairs; };

    struct Expr : std::variant<std::monostate, ExprLiteral, ExprId, ExprBinop, 
                                ExprUnop, ExprFunc, ExprIndex> {
        using variant::variant;
    };

    // Statement types
    struct StmtExpression  { ExprPtr expression; };
    struct StmtAssign      { std::string name; Lexer::Token op; ExprPtr initializer; };
    struct StmtBlock       { std::vector<Stmt> stmts; };
    struct StmtIf          { ExprPtr condition; StmtPtr body; };
    struct StmtFor         { ExprPtr condition; StmtPtr body; };
    struct StmtWhile       { ExprPtr condition; StmtPtr body; };
    struct StmtFunc        { std::string name; std::vector<std::string> args; 
                             std::vector<TypeName> argTypes; std::vector<StmtPtr> body; };
    struct StmtReturn      { ExprPtr value; };
    struct StmtContinue    {};
    struct StmtBreak       {};
    struct StmtPass        {};
    // TODO:
    // add classes and imports

    struct Stmt : std::variant<std::monostate, StmtExpression, StmtAssign, StmtBlock, StmtIf, 
        StmtFor, StmtWhile, StmtFunc, StmtReturn, StmtContinue, StmtBreak, StmtPass> {
        using variant::variant;
    };

    class Parser {
    public:
        Parser(std::vector<Lexer::Token> tokens);
        std::vector<StmtPtr> *parse(std::vector<Lexer::Token> tokens);
        void print_stmts();    
    private:
        size_t index = 0;
        size_t parenCount = 0;
        std::vector<Lexer::Token> tokens;
        std::vector<StmtPtr> stmts;

        StmtPtr parse_stmt();

        ExprPtr parse_expr();

        // TODO:
        // finish unimplemented things

        // order of operations 
        ExprPtr parse_paren_expr();
        ExprPtr parse_callget_expr(); // id() id.id id[] id[:]
        ExprPtr parse_var_expr(); // literals, id
        ExprPtr parse_pow_expr(); // **
        ExprPtr parse_unop_expr(); // ~ -
        ExprPtr parse_multdiv_expr(); // * // / %
        ExprPtr parse_addsub_expr();
        ExprPtr parse_bit_shift_expr();
        ExprPtr parse_bit_and_expr();
        ExprPtr parse_bit_xor_expr();
        ExprPtr parse_bit_or_expr();
        ExprPtr parse_cmp_expr();
        //ExprPtr parse_assign_expr();
        ExprPtr parse_not_expr();
        ExprPtr parse_and_expr();
        ExprPtr parse_or_expr();
        // ExprPtr parse_ternary_expr();
        // ExprPtr parse_lambda_expr();

        // returns the next token or EndOfFile if end is reached
        Lexer::Token *lookahead(size_t increment = 1);
        Lexer::Token *get(size_t i);
    };
}

