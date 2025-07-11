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

    using ExprPtr = std::shared_ptr<Expr>;
    using StmtPtr = std::shared_ptr<Stmt>;

    // Expression types
    struct ExprLiteral     { std::variant<int, std::string, bool> value; };
    struct ExprVariable    { std::string name; };
    struct ExprBinary      { ExprPtr left; Lexer::Token op; ExprPtr right; };
    struct ExprGrouping    { ExprPtr expression; };

    struct Expr : std::variant<ExprLiteral, ExprVariable, ExprBinary, ExprGrouping> {
        using variant::variant;
    };

    // Statement types
    struct StmtExpression  { ExprPtr expression; };
    struct StmtVarDecl     { std::string name; ExprPtr initializer; };

    struct Stmt : std::variant<StmtExpression, StmtVarDecl> {
        using variant::variant;
    };

    std::vector<Stmt> parse(std::vector<Lexer::Token> tokens); 
}