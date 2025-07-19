#pragma once

#include "optimizer.hpp"

namespace Generator {
    inline std::unordered_map<Lexer::Type, std::string> op_to_string = {
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

    std::unordered_map<Parser::TypeName, std::string> typename_to_string() = {
        {Parser::TypeName::Int, "int"},
        {Parser::TypeName::Float, "float"},
        {Parser::TypeName::Bool, "bool"},
        {Parser::TypeName::List, "std::vector<bruh>"},
        {Parser::TypeName::Dict, "std::unordered_map<bruh>"},
        {Parser::TypeName::Tuple, "std::tuple<bruh,bruh>"},
        {Parser::TypeName::Set, "std::unordered_set<bruh>"},
        {Parser::TypeName::Range, "auto"},
        {Parser::TypeName::Enumerate, "auto"},
        {Parser::TypeName::Zip, "auto"},
        {Parser::TypeName::Slice, "auto"},
        {Parser::TypeName::Frozenset, "auto"},
        {Parser::TypeName::Function, "auto"},
        {Parser::TypeName::Lambda, "auto"},
        {Parser::TypeName::Type, "typename"},
        {Parser::TypeName::Any, "bruh"}
    };

    class Generator {
    public:
        Generator(std::vector<Parser::StmtPtr> stmts) : stmts(stmts) {};
        std::string generate();
        std::string generate_stmt(Parser::StmtPtr stmt);
        std::string generate_expr(Parser::ExprPtr expr);
    private:
        std::vector<Parser::StmtPtr> stmts;
        size_t index = 0;
        size_t tab_count = 0;
        std::string code;

        std::string generate_assign_stmt(Parser::StmtPtr stmt);
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