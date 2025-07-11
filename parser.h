#pragma once

#include <string>
#include <variant>
#include <optional>
#include <vector>

#include "lexer.h"

namespace Parser {
    // Forward declarations
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

    struct Expr : std::variant<ExprLiteral, ExprId, ExprBinop, ExprUnop, ExprFunc> {
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

    struct Stmt : std::variant<StmtExpression, StmtAssign, StmtBlock, StmtIf, 
        StmtFor, StmtWhile, StmtFunc, StmtReturn, StmtContinue, StmtBreak> {
        using variant::variant;
    };

    

    class Parser {
    public:
        Parser(std::vector<Lexer::Token> tokens);
        std::vector<Stmt> parse(std::vector<Lexer::Token> tokens);
    private:
        int index = 0;
        std::vector<Lexer::Token> tokens;
        
        Stmt parse_stmt(size_t start, size_t end);
        Expr parse_expr(size_t start, size_t end);
        // returns the next token or EndOfFile if end is reached
        Lexer::Token lookahead(int increment = 1);
        
    };
}