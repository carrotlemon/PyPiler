#pragma once

#include <string>
#include <variant>
#include <optional>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "lexer.h"

namespace Parser {
    std::unordered_map<Lexer::TokenType, int> OpPrecedence = {
        {},
        {},
        {},
        {},
        {},
        {},
        {},
                
    };
    std::unordered_set<Lexer::TokenType> tokenLiteral = {
        Lexer::TokenType::Float, Lexer::TokenType::Int, 
        Lexer::TokenType::String, Lexer::TokenType::None,
        Lexer::TokenType::True, Lexer::TokenType::False 
    };
    std::unordered_set<Lexer::TokenType> tokenBinop = {
        Lexer::TokenType::DSlash, Lexer::TokenType::Slash, 
        Lexer::TokenType::Plus, Lexer::TokenType::Minus, 
        Lexer::TokenType::Star, Lexer::TokenType::Period,
        Lexer::TokenType::And, Lexer::TokenType::Or,
        Lexer::TokenType::BitOr, Lexer::TokenType::BitAnd, 
        Lexer::TokenType::ShRight, Lexer::TokenType::ShLeft, 
        Lexer::TokenType::Xor, Lexer::TokenType::In,
        Lexer::TokenType::Is
    };
    std::unordered_set<Lexer::TokenType> tokenUnop = {
        Lexer::TokenType::Negate, Lexer::TokenType::Not,
        Lexer::TokenType::Minus
    };
    // std::unordered_set<Lexer::TokenType> tokenEndExpr = {
    //     Lexer::TokenType::Colon, Lexer::TokenType::Newline,
    //     Lexer::TokenType::Comma
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
    struct ExprId          { std::string id; };
    struct ExprBinop       { ExprPtr left; Lexer::Token op; ExprPtr right; };
    struct ExprUnop        { Lexer::Token op; ExprPtr expr; };
    struct ExprFunc        { ExprId name; std::vector<Expr> args; StmtPtr body; };

    struct Expr : std::variant<std::monostate, ExprLiteral, ExprId, ExprBinop, ExprUnop, ExprFunc> {
        using variant::variant;
    };

    // Statement types
    struct StmtExpression  { ExprPtr expression; };
    struct StmtAssign      { std::string name; Lexer::Token op; ExprPtr initializer; };
    struct StmtBlock       { std::vector<Stmt> stmts; };
    struct StmtIf          { ExprPtr condition; StmtPtr body; };
    struct StmtFor         { ExprPtr condition; StmtPtr body; };
    struct StmtWhile       { ExprPtr condition; StmtPtr body; };
    struct StmtFunc        { std::string name; std::vector<std::string> argsNames; std::vector<TypeName> argTypes; };
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
        std::vector<Stmt> *parse(std::vector<Lexer::Token> tokens);
        void print_stmts();    
    private:
        size_t index = 0;
        std::vector<Lexer::Token> tokens;
        std::vector<Stmt> stmts;

        Stmt parse_stmt();
        Expr parse_expr(bool in_paren = false);
        // returns the next token or EndOfFile if end is reached
        Lexer::Token *lookahead(size_t increment = 1);
        Lexer::Token *get(size_t i);
    };
}

