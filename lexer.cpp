#include <regex>
#include <iostream>
#include "lexer.h"

namespace Lexer {
    Lexer::Lexer(std::string *source, size_t tab_stop) : source(source), tab_stop(tab_stop) {};

    std::tuple<TokenPtr, size_t, int> Lexer::findMatch(size_t index, int line) {
        std::smatch match;
        std::string sub = source->substr(index);
        Token token;
        size_t newIndex = -1;
        // Only reassign token.type and token.literal
        if (regex_search(sub, match, re_comment)) { token.type=Type::Comment; }
        else if (regex_search(sub, match, re_scope)) {
            token.type = Type::Scope; 
            int scope = 0;
            for(size_t i = 1; i < sub.size(); ++i) {
                if(sub[i] == ' ') {
                    ++scope;
                } else if (sub[i] == '\t'){
                    while(scope & (tab_stop-1)) {
                        ++scope;
                    }
                } else {
                    break;
                }
            }
            token.literal = scope;
        }
        else if (regex_search(sub, match, re_ws)) { token.type=Type::Whitespace; }
        else if (regex_search(sub, match, re_arrow)) { token.type=Type::Arrow; }
        else if (regex_search(sub, match, re_bitor)) { token.type=Type::BitOr; }
        else if (regex_search(sub, match, re_bitand)) { token.type=Type::BitAnd; }
        else if (regex_search(sub, match, re_shright)) { token.type=Type::ShRight; }
        else if (regex_search(sub, match, re_shleft)) { token.type=Type::ShLeft; }
        else if (regex_search(sub, match, re_negate)) { token.type=Type::Negate; }
        else if (regex_search(sub, match, re_xor)) { token.type=Type::Xor; }
        else if (regex_search(sub, match, re_dslash)) { token.type=Type::DSlash; }
        else if (regex_search(sub, match, re_slash)) { token.type=Type::Slash; }
        else if (regex_search(sub, match, re_mod)) { token.type=Type::Mod; }
        else if (regex_search(sub, match, re_plus)) { token.type=Type::Plus; }
        else if (regex_search(sub, match, re_minus)) { token.type=Type::Minus; }
        else if (regex_search(sub, match, re_dstar)) { token.type=Type::DStar; }
        else if (regex_search(sub, match, re_star)) { token.type=Type::Star; }
        else if (regex_search(sub, match, re_colon)) { token.type=Type::Colon; }
        else if (regex_search(sub, match, re_comma)) { token.type=Type::Comma; }
        else if (regex_search(sub, match, re_period)) { token.type=Type::Period; }
        else if (regex_search(sub, match, re_lpar)) { token.type=Type::LPar; ++parenCount; }
        else if (regex_search(sub, match, re_rpar)) { token.type=Type::RPar; --parenCount; }
        else if (regex_search(sub, match, re_lsquare)) { token.type=Type::LSquare; ++parenCount; }
        else if (regex_search(sub, match, re_rsquare)) { token.type=Type::RSquare; --parenCount; }
        else if (regex_search(sub, match, re_lbrace)) { token.type=Type::LBrace; ++parenCount; }
        else if (regex_search(sub, match, re_rbrace)) { token.type=Type::RBrace; --parenCount; }
        else if (regex_search(sub, match, re_greater_equal)) { token.type=Type::GreaterEqual; }
        else if (regex_search(sub, match, re_greater)) { token.type=Type::Greater; }
        else if (regex_search(sub, match, re_less_equal)) { token.type=Type::LessEqual; }
        else if (regex_search(sub, match, re_less)) { token.type=Type::Less; }
        else if (regex_search(sub, match, re_not_equal)) { token.type=Type::NotEqual; }
        else if (regex_search(sub, match, re_equal)) { token.type=Type::Equal; }
        else if (regex_search(sub, match, re_assign)) { token.type=Type::Assign; }
        else if (regex_search(sub, match, re_if)) { token.type=Type::If; }
        else if (regex_search(sub, match, re_elif)) { token.type=Type::Elif; }
        else if (regex_search(sub, match, re_else)) { token.type=Type::Else; }
        else if (regex_search(sub, match, re_def)) { token.type=Type::Def; }
        else if (regex_search(sub, match, re_or)) { token.type=Type::Or; }
        else if (regex_search(sub, match, re_and)) { token.type=Type::And; }
        else if (regex_search(sub, match, re_not)) { token.type=Type::Not; }
        else if (regex_search(sub, match, re_return)) { token.type=Type::Return; }
        else if (regex_search(sub, match, re_continue)) { token.type=Type::Continue; }
        else if (regex_search(sub, match, re_break)) { token.type=Type::Break; }
        else if (regex_search(sub, match, re_for)) { token.type=Type::For; }
        else if (regex_search(sub, match, re_while)) { token.type=Type::While; }
        else if (regex_search(sub, match, re_in)) { token.type=Type::In; }
        else if (regex_search(sub, match, re_is)) { token.type=Type::Is; }
        else if (regex_search(sub, match, re_float)) { token.type=Type::Float; token.literal=std::stod(match.str());}
        else if (regex_search(sub, match, re_int)) { token.type=Type::Int; token.literal=std::stoi(match.str());}
        else if (regex_search(sub, match, re_string)) { token.type=Type::String; token.literal=match.str();}
        else if (regex_search(sub, match, re_true)) { token.type=Type::True; token.literal=true;}
        else if (regex_search(sub, match, re_false)) { token.type=Type::False; token.literal=false;}
        else if (regex_search(sub, match, re_none)) { token.type=Type::None; token.literal=std::monostate();}
        else if (regex_search(sub, match, re_id)) { token.type=Type::Id; token.literal=match.str();}
        newIndex = index + match.str().length();
        token.lexeme=match.str();
        TokenPtr tokenptr = std::make_shared<Token>(token);
        return {tokenptr, newIndex, line};
    }

    void Lexer::printTokens(bool advanced) {
        size_t i = 0;
        
        for(auto t : tokens) {
            if(t->type == Type::Comment) { std::cout << "Comment"; }
            else if(t->type == Type::Tab) { std::cout << "Tab"; }
            else if(t->type == Type::Newline) { std::cout << "Newline"; }
            else if(t->type == Type::Whitespace) { std::cout << "Whitespace"; }
            else if(t->type == Type::Arrow) { std::cout << "Arrow"; }
            else if(t->type == Type::BitOr) { std::cout << "BitOr"; }
            else if(t->type == Type::BitAnd) { std::cout << "BitAnd"; }
            else if(t->type == Type::ShRight) { std::cout << "ShRight"; }
            else if(t->type == Type::ShLeft) { std::cout << "ShLeft"; }
            else if(t->type == Type::Negate) { std::cout << "Negate"; }
            else if(t->type == Type::Xor) { std::cout << "Xor"; }
            else if(t->type == Type::DSlash) { std::cout << "DSlash"; }
            else if(t->type == Type::Slash) { std::cout << "Slash"; }
            else if(t->type == Type::Plus) { std::cout << "Plus"; }
            else if(t->type == Type::Minus) { std::cout << "Minus"; }
            else if(t->type == Type::Star) { std::cout << "Star"; }
            else if(t->type == Type::Colon) { std::cout << "Colon"; }
            else if(t->type == Type::Comma) { std::cout << "Comma"; }
            else if(t->type == Type::Period) { std::cout << "Period"; }   
            else if(t->type == Type::LPar) { std::cout << "LPar"; }
            else if(t->type == Type::RPar) { std::cout << "RPar"; }
            else if(t->type == Type::LSquare) { std::cout << "LSquare"; }
            else if(t->type == Type::RSquare) { std::cout << "RSquare"; }
            else if(t->type == Type::LBrace) { std::cout << "LBrace"; }
            else if(t->type == Type::RBrace) { std::cout << "RBrace"; }
            else if(t->type == Type::GreaterEqual) { std::cout << "GreaterEqual"; }
            else if(t->type == Type::Greater) { std::cout << "Greater"; }
            else if(t->type == Type::LessEqual) { std::cout << "LessEqual"; }
            else if(t->type == Type::Less) { std::cout << "Less"; }
            else if(t->type == Type::NotEqual) { std::cout << "NotEqual"; }
            else if(t->type == Type::Equal) { std::cout << "Equal"; }
            else if(t->type == Type::Assign) { std::cout << "Assign"; }

            else if(t->type == Type::If) { std::cout << "If"; }
            else if(t->type == Type::Elif) { std::cout << "Elif"; }
            else if(t->type == Type::Else) { std::cout << "Else"; }
            else if(t->type == Type::Def) { std::cout << "Def"; }
            else if(t->type == Type::Or) { std::cout << "Or"; }
            else if(t->type == Type::And) { std::cout << "And"; }
            else if(t->type == Type::Not) { std::cout << "Not"; }
            else if(t->type == Type::Return) { std::cout << "Return"; }
            else if(t->type == Type::Continue) { std::cout << "Continue"; }
            else if(t->type == Type::Break) { std::cout << "Br"; }
            else if(t->type == Type::For) { std::cout << "For"; }
            else if(t->type == Type::While) { std::cout << "While"; }
            else if(t->type == Type::In) { std::cout << "In"; }

            else if(t->type == Type::Float) { std::cout << "Float"; }
            else if(t->type == Type::Int) { std::cout << "Int"; }
            else if(t->type == Type::String) { std::cout << "String"; }
            else if(t->type == Type::True) { std::cout << "True"; }
            else if(t->type == Type::False) { std::cout << "False"; }
            else if(t->type == Type::None) { std::cout << "None"; }
            else if(t->type == Type::Id) { std::cout << "Id"; }
            else if(t->type == Type::EndOfFile) { std::cout << "EndOfFile"; }
            else { std::cout << "INVALID"; }
            
            if(i < tokens.size()-1) { std::cout << ", "; }
            if(t->type == Type::Newline) { std::cout << std::endl; }
            ++i;
        }
        i = 0;
        if (advanced) {
            std::cout << "\n" << std::endl;
            for(auto t : tokens) {
                std::cout << t->lexeme;
                if(i < tokens.size()-1 && t->type != Type::Newline) { std::cout << ", "; }
                ++i;
            }
        }
        std::cout << std::endl;
    }

    std::vector<TokenPtr> *Lexer::tokenize() {
        size_t index = 0;
        int line = 0;
        while(index != -1 && index < source->size()) {
            auto [token, i, newline] = findMatch(index, line);
            if (!(token->type == Type::Whitespace || parenCount == 0 && token->type == Type::Newline)) {
                tokens.push_back(token);
            }
            index = i;
            line = newline;
        }
        tokens.push_back(std::make_shared<Token>(Token{Type::EndOfFile, "EOF"}));
        return &tokens;
    }
}