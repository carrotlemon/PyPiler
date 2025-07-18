#pragma once

#include "parser.hpp"

namespace Optimizer {

    class Optimizer {
    public:
        Optimizer(std::vector<Parser::StmtPtr> stmts);
        std::vector<Parser::StmtPtr> optimize();
    private:
        std::vector<Parser::StmtPtr> stmts;
    };
}