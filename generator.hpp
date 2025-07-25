#pragma once

#include "optimizer.hpp"

namespace Generator {
    inline std::unordered_map<Lexer::Type, std::string> op_to_string = {
        {Lexer::Type::And, "&&"},
        {Lexer::Type::Or, "||"},
        {Lexer::Type::BitOr, "|"},
        {Lexer::Type::BitAnd, "&"},
        {Lexer::Type::ShRight, ">>"},
        {Lexer::Type::ShLeft, "<<"},
        {Lexer::Type::Xor, "^"},
        {Lexer::Type::Negate, "~"},
        {Lexer::Type::Slash, "/"},
        {Lexer::Type::Mod, "%"},
        {Lexer::Type::Plus, "+"},
        {Lexer::Type::Minus, "-"},
        {Lexer::Type::Star, "*"},
        {Lexer::Type::Period, "."},
        {Lexer::Type::Comma, "+"},
        {Lexer::Type::GreaterEqual, ">="},
        {Lexer::Type::Greater, ">"},
        {Lexer::Type::LessEqual, "<="},
        {Lexer::Type::Less, "<"},
        {Lexer::Type::NotEqual, "!="},
        {Lexer::Type::Equal, "=="},
        {Lexer::Type::Assign, "="},
        {Lexer::Type::True, "true"},
        {Lexer::Type::False, "false"},
        {Lexer::Type::None, "nullptr"},
    };

    // inline std::unordered_map<Parser::TypeNameEnum, std::string> typename_to_string = {
    //     {Parser::TypeNameEnum::Int, "int"},
    //     {Parser::TypeNameEnum::Float, "float"},
    //     {Parser::TypeNameEnum::Bool, "bool"},
    //     {Parser::TypeNameEnum::List, "std::vector<bruh>"},
    //     {Parser::TypeNameEnum::Dict, "std::unordered_map<bruh>"},
    //     {Parser::TypeNameEnum::Tuple, "std::tuple<bruh,bruh>"},
    //     {Parser::TypeNameEnum::Set, "std::unordered_set<bruh>"},
    //     {Parser::TypeNameEnum::Range, "auto"},
    //     {Parser::TypeNameEnum::Enumerate, "auto"},
    //     {Parser::TypeNameEnum::Zip, "auto"},
    //     {Parser::TypeNameEnum::Slice, "auto"},
    //     {Parser::TypeNameEnum::Frozenset, "auto"},
    //     {Parser::TypeNameEnum::Function, "auto"},
    //     {Parser::TypeNameEnum::Lambda, "auto"},
    //     {Parser::TypeNameEnum::Type, "typename"},
    //     {Parser::TypeNameEnum::Any, "bruh"}
    // };

    class Generator {
    public:
        Generator(std::vector<Parser::StmtPtr> stmts) : stmts(stmts) {};
        std::string generate();
        std::string generate_stmt(Parser::StmtPtr stmt);
        std::string generate_expr(Parser::ExprPtr expr);
    private:
        std::vector<std::unordered_set<std::string>> env = {std::unordered_set<std::string>{"hi"}};
        std::vector<Parser::StmtPtr> stmts;
        size_t scope = 0;
        std::string code;

        std::string typename_to_string(Parser::TypeNamePtr);

        std::string generate_assign_stmt(Parser::StmtPtr stmt);
        std::string generate_expr_stmt(Parser::StmtPtr stmt);
        std::string generate_block_stmt(Parser::StmtPtr stmt);
        std::string generate_if_stmt(Parser::StmtPtr stmt);
        std::string generate_for_stmt(Parser::StmtPtr stmt);
        std::string generate_while_stmt(Parser::StmtPtr stmt);
        std::string generate_func_stmt(Parser::StmtPtr stmt);
        std::string generate_return_stmt(Parser::StmtPtr stmt);
        std::string generate_continue_stmt(Parser::StmtPtr stmt);
        std::string generate_break_stmt(Parser::StmtPtr stmt);
        std::string generate_pass_stmt(Parser::StmtPtr stmt);
        std::string generate_comment_stmt(Parser::StmtPtr stmt);

        std::string generate_literal_expr(Parser:: ExprPtr expr);
        std::string generate_id_expr(Parser:: ExprPtr expr);
        std::string generate_binop_expr(Parser:: ExprPtr expr);
        std::string generate_unop_expr(Parser:: ExprPtr expr);
        std::string generate_func_expr(Parser:: ExprPtr expr);
        std::string generate_index_expr(Parser:: ExprPtr expr);
        std::string generate_list_expr(Parser:: ExprPtr expr);
        std::string generate_tuple_expr(Parser:: ExprPtr expr);
        std::string generate_dict_expr(Parser:: ExprPtr expr);
    };
} 