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
        
    };
}