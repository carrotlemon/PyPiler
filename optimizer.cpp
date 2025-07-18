#include "optimizer.hpp"

namespace Optimizer {
    Optimizer::Optimizer(std::vector<Parser::StmtPtr> stmts) : stmts(stmts) {};

    std::vector<Parser::StmtPtr> Optimizer::optimize() {
        return stmts;
    };
}