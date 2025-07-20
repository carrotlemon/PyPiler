#include "generator.hpp"

namespace Generator {
    std::string Generator::generate() {
        for(Parser::StmtPtr stmt : stmts) {
            code += generate_stmt(stmt);
        }
    }

    std::string Generator::generate_stmt(Parser::StmtPtr stmt) {
        std::string res;
        if(std::holds_alternative<Parser::StmtAssign>(*stmt)) { res = generate_assign_stmt(stmt); }
        else if(std::holds_alternative<Parser::StmtBlock>(*stmt)) { res = generate_block_stmt(stmt); }
        else if(std::holds_alternative<Parser::StmtIf>(*stmt)) { res = generate_if_stmt(stmt); }
        else if(std::holds_alternative<Parser::StmtFor>(*stmt)) { res = generate_for_stmt(stmt); }
        else if(std::holds_alternative<Parser::StmtWhile>(*stmt)) { res = generate_while_stmt(stmt); }
        else if(std::holds_alternative<Parser::StmtFunc>(*stmt)) { res = generate_func_stmt(stmt); }
        else if(std::holds_alternative<Parser::StmtReturn>(*stmt)) { res = generate_return_stmt(stmt); }
        else if(std::holds_alternative<Parser::StmtContinue>(*stmt)) { res = generate_continue_stmt(stmt); }
        else if(std::holds_alternative<Parser::StmtBreak>(*stmt)) { res = generate_break_stmt(stmt); }
        else if(std::holds_alternative<Parser::StmtPass>(*stmt)) { res = generate_pass_stmt(stmt); }
        else if(std::holds_alternative<Parser::StmtComment>(*stmt)) { res = generate_comment_stmt(stmt); }
        return res;
    }
    
    std::string Generator::generate_assign_stmt(Parser::StmtPtr stmt) {
        Parser::StmtAssign *curr = &std::get<Parser::StmtAssign>(*stmt);
        return generate_expr(curr->expr) + " " + op_to_string[curr->op->type] + " " + generate_expr(curr->expr) = ";\n";
    }
    std::string Generator::generate_block_stmt(Parser::StmtPtr stmt) {
        Parser::StmtBlock *curr = &std::get<Parser::StmtBlock>(*stmt);
        std::string res;
        std::string tabs;
        for (size_t i = 0; i < tab_count; ++i) {
            tabs += "\t";        
        }
        for(Parser::StmtPtr stmt : curr->stmts) {
            res += tabs + generate_stmt(stmt);
        }
        return res;
    }
    std::string Generator::generate_if_stmt(Parser::StmtPtr stmt) {
        Parser::StmtIf *curr = &std::get<Parser::StmtIf>(*stmt);
        std::string res;
        for (size_t i = 0; i < curr->conditions.size(); ++i) {
            if (i == 0) {
                res += "if(" + generate_expr(curr->conditions[i]) + ") {\n";
            } else {
                res += "else if(" + generate_expr(curr->conditions[i]) + ") {\n";
            }
            ++tab_count;
            res += "\t" + generate_stmt(curr->bodies[i]);
            --tab_count;
            res += "}";
        }
        if(curr->conditions.size() < curr->bodies.size()) {
            res += "else {\n" + generate_block_stmt(curr->bodies[curr->bodies.size()-1]) + "}\n";
        }
        return res;
    }
    std::string Generator::generate_for_stmt(Parser::StmtPtr stmt) {
        Parser::StmtFor *curr = &std::get<Parser::StmtFor>(*stmt);
        // for i, n in range(7):
        // for 
        std::string res = "for(auto ";
        for(auto expr : curr->targets) {
            res += generate_expr(expr) + ", ";
        }
        res += " : " + generate_expr(curr->iterable) + ") {\n";
        ++tab_count;
        res += generate_block_stmt(curr->body) + "}\n";
        --tab_count;
        return res;
    }
    std::string Generator::generate_while_stmt(Parser::StmtPtr stmt) {
        Parser::StmtWhile *curr = &std::get<Parser::StmtWhile>(*stmt);
        std::string res = "while(" + generate_expr(curr->condition) + ") {\n";
        ++tab_count;
        res += generate_block_stmt(curr->body) + "}\n";
        --tab_count;
        return res;
    }
    std::string Generator::generate_func_stmt(Parser::StmtPtr stmt) {
        // Parser::StmtFunc *curr = &std::get<Parser::StmtFunc>(*stmt);
        // std::string res = curr->name + "(";
        // for(size_t i = 0; i < curr->args.size(); ++i) {
        //     Parser::TypeName type_name = std::get<1>(curr->args[i]);
        //     std::string type = Parser::type_to_string(type_name);
        //     if(type_name == Parser::TypeName::Any) {
        //         type = "";
        //     }
        //     std::string id = std::get<0>(curr->args[i]);
        //     if(type == Parser::TypeName::Any) {
                
        //     }
        //     if(i != curr->args.size() - 1) {
        //         res += ", ";
        //     }
        // }
        // return res;
        return "";
    }
    std::string Generator::generate_return_stmt(Parser::StmtPtr stmt) {
        Parser::StmtReturn *curr = &std::get<Parser::StmtReturn>(*stmt);
        return "return " + generate_expr(curr->value) + ";\n";
    }
    std::string Generator::generate_continue_stmt(Parser::StmtPtr stmt) {
        return "continue\n";
    }
    std::string Generator::generate_break_stmt(Parser::StmtPtr stmt) {
        return "break\n";
    }
    std::string Generator::generate_pass_stmt(Parser::StmtPtr stmt) {
        return "pass\n";
    }
    std::string Generator::generate_comment_stmt(Parser::StmtPtr stmt) {
        Parser::StmtComment *curr = &std::get<Parser::StmtComment>(*stmt);
        return "//" + curr->comment->lexeme.substr(1);
    }

    std::string Generator::generate_expr(Parser::ExprPtr expr) {
        std::string res;
        if(std::holds_alternative<Parser::ExprLiteral>(*expr)) { res = generate_literal_expr(expr); }
        else if (std::holds_alternative<Parser::ExprId>(*expr)) { res = generate_id_expr(expr); }
        else if (std::holds_alternative<Parser::ExprBinop>(*expr)) { res = generate_binop_expr(expr); }
        else if (std::holds_alternative<Parser::ExprUnop>(*expr)) { res = generate_unop_expr(expr); }
        else if (std::holds_alternative<Parser::ExprFunc>(*expr)) { res = generate_func_expr(expr); }
        else if (std::holds_alternative<Parser::ExprIndex>(*expr)) { res = generate_index_expr(expr); }
        else if (std::holds_alternative<Parser::ExprList>(*expr)) { res = generate_list_expr(expr); }
        else if (std::holds_alternative<Parser::ExprTuple>(*expr)) { res = generate_tuple_expr(expr); }
        else if (std::holds_alternative<Parser::ExprDict>(*expr)) { res = generate_dict_expr(expr); }
        return res;
    }

    std::string Generator::generate_literal_expr(Parser:: ExprPtr expr) {
        Parser::ExprLiteral *curr = &std::get<Parser::ExprLiteral>(*expr);
        std::string res;
        if (curr->value->type == Lexer::Type::True) { res = "true"; }
        else if (curr->value->type == Lexer::Type::False) { res = "false"; }
        else if (curr->value->type == Lexer::Type::None) { res = "nullptr"; }
        else { res = curr->value->lexeme; }
        return res;
    }

    std::string Generator::generate_id_expr(Parser:: ExprPtr expr) {
        Parser::ExprId *curr = &std::get<Parser::ExprId>(*expr);
        return curr->id->lexeme;
    }
    std::string Generator::generate_binop_expr(Parser:: ExprPtr expr) {
        Parser::ExprBinop *curr = &std::get<Parser::ExprBinop>(*expr);
        std::string res;
        if(curr->op->type == Lexer::Type::Period) {
            res = generate_expr(curr->left) + op_to_string[curr->op->type] + generate_expr(curr->right);
        } else {
            res = generate_expr(curr->left) + " " + op_to_string[curr->op->type] + " " + generate_expr(curr->right);
        }
        return res;
    }
    std::string Generator::generate_unop_expr(Parser:: ExprPtr expr) {
        Parser::ExprUnop *curr = &std::get<Parser::ExprUnop>(*expr);
        return op_to_string[curr->op->type] + generate_expr(curr->expr);
    }
    std::string Generator::generate_func_expr(Parser:: ExprPtr expr) {
        return "";
    }
    std::string Generator::generate_index_expr(Parser:: ExprPtr expr) {
        Parser::ExprIndex *curr = &std::get<Parser::ExprIndex>(*expr);
        std::string res = generate_id_expr(curr->id) + "[";
        for(size_t i = 0; i < curr->args.size(); ++i) {
            res += generate_expr(curr->args[i]);
            if(i != curr->args.size()-1) {
                res += ",";
            }
        }
        res += "]";
        return res;
    }
    std::string Generator::generate_list_expr(Parser:: ExprPtr expr) {
        Parser::ExprList *curr = &std::get<Parser::ExprList>(*expr);
        std::string res = "std::vector{";
        for(size_t i = 0; i < curr->elements.size(); ++i) {
            res += generate_expr(curr->elements[i]);
            if(i < curr->elements.size()-1) {
                res += ", ";
            }
        }
        res += "}";
        return res;
    }
    std::string Generator::generate_tuple_expr(Parser:: ExprPtr expr) {
        Parser::ExprTuple *curr = &std::get<Parser::ExprTuple>(*expr);
        std::string res = "std::tuple";
        // add types here
        res += ">{";
        for(size_t i = 0; i < curr->elements.size(); ++i) {
            res += generate_expr(curr->elements[i]);
            if(i < curr->elements.size()-1) {
                res += ", ";
            }
        }
        return res;
    }
    std::string Generator::generate_dict_expr(Parser:: ExprPtr expr) {
        Parser::ExprDict *curr = &std::get<Parser::ExprDict>(*expr);
        std::string res = "std::unordered_map<";
        // add types here
        res += ">{";
        for(size_t i = 0; i < curr->pairs.size(); ++i) {
            res += "{" + generate_expr(std::get<0>(curr->pairs[i])) + ", " + generate_expr(std::get<1>(curr->pairs[i])) + "}";
            if(i < curr->pairs.size()-1) {
                res += ", ";
            }
        }
        return res + "}";
    }
}