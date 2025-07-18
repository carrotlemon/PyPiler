#pragma once

#include "optimizer.hpp"

namespace Generator {
    
    
    class Generator {
    public:
        Generator(std::vector<Parser::StmtPtr> stmts) : stmts(stmts) {};
        std::string generate();
    private:
        std::vector<Parser::StmtPtr> stmts;
        std::string code;

        std::string generate_assign_stmt();
        std::string generate_block_stmt();
        std::string generate_if_stmt();
        std::string generate_for_stmt();
        std::string generate_while_stmt();
        std::string generate_func_stmt();
        std::string generate_return_stmt();
        std::string generate_continue_stmt();
        std::string generate_break_stmt();
        std::string generate_pass_stmt();
        std::string generate_comment_stmt();
    };
}