#include <regex>
#include <iostream>
#include "lexer.hpp"

namespace Lexer {
    Lexer::Lexer(std::string *source, size_t tab_stop) : tab_stop(tab_stop), source(source) {};

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
            if(tokens.back()->type == Type::Scope) {
                tokens.pop_back();
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

    void Lexer::print_tokens(bool advanced) {
        size_t i = 0;
        
        for(auto t : tokens) {
            std::cout << token_to_string(t);
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

    std::string token_to_string(TokenPtr token) {
        if(token->type == Type::Comment) { return "Comment"; }
        else if(token->type == Type::Scope) { return "Scope"; }
        else if(token->type == Type::Newline) { return "Newline"; }
        else if(token->type == Type::Whitespace) { return "Whitespace"; }
        else if(token->type == Type::Arrow) { return "Arrow"; }
        else if(token->type == Type::BitOr) { return "BitOr"; }
        else if(token->type == Type::BitAnd) { return "BitAnd"; }
        else if(token->type == Type::ShRight) { return "ShRight"; }
        else if(token->type == Type::ShLeft) { return "ShLeft"; }
        else if(token->type == Type::Negate) { return "Negate"; }
        else if(token->type == Type::Xor) { return "Xor"; }
        else if(token->type == Type::DSlash) { return "DSlash"; }
        else if(token->type == Type::Slash) { return "Slash"; }
        else if(token->type == Type::Plus) { return "Plus"; }
        else if(token->type == Type::Minus) { return "Minus"; }
        else if(token->type == Type::Star) { return "Star"; }
        else if(token->type == Type::Colon) { return "Colon"; }
        else if(token->type == Type::Comma) { return "Comma"; }
        else if(token->type == Type::Period) { return "Period"; }   
        else if(token->type == Type::LPar) { return "LPar"; }
        else if(token->type == Type::RPar) { return "RPar"; }
        else if(token->type == Type::LSquare) { return "LSquare"; }
        else if(token->type == Type::RSquare) { return "RSquare"; }
        else if(token->type == Type::LBrace) { return "LBrace"; }
        else if(token->type == Type::RBrace) { return "RBrace"; }
        else if(token->type == Type::GreaterEqual) { return "GreaterEqual"; }
        else if(token->type == Type::Greater) { return "Greater"; }
        else if(token->type == Type::LessEqual) { return "LessEqual"; }
        else if(token->type == Type::Less) { return "Less"; }
        else if(token->type == Type::NotEqual) { return "NotEqual"; }
        else if(token->type == Type::Equal) { return "Equal"; }
        else if(token->type == Type::Assign) { return "Assign"; }

        else if(token->type == Type::If) { return "If"; }
        else if(token->type == Type::Elif) { return "Elif"; }
        else if(token->type == Type::Else) { return "Else"; }
        else if(token->type == Type::Def) { return "Def"; }
        else if(token->type == Type::Or) { return "Or"; }
        else if(token->type == Type::And) { return "And"; }
        else if(token->type == Type::Not) { return "Not"; }
        else if(token->type == Type::Return) { return "Return"; }
        else if(token->type == Type::Continue) { return "Continue"; }
        else if(token->type == Type::Break) { return "Br"; }
        else if(token->type == Type::For) { return "For"; }
        else if(token->type == Type::While) { return "While"; }
        else if(token->type == Type::In) { return "In"; }

        else if(token->type == Type::Float) { return "Float"; }
        else if(token->type == Type::Int) { return "Int"; }
        else if(token->type == Type::String) { return "String"; }
        else if(token->type == Type::True) { return "True"; }
        else if(token->type == Type::False) { return "False"; }
        else if(token->type == Type::None) { return "None"; }
        else if(token->type == Type::Id) { return "Id"; }
        else if(token->type == Type::EndOfFile) { return "EndOfFile"; }
        else { return "INVALID"; }
    }

    std::vector<TokenPtr> *Lexer::tokenize() {
        int index = 0;
        int line = 0;
        Token init(Type::Scope, "", 0, 0);
        tokens.push_back(std::make_shared<Token>(init));
        while(index != -1 && index < (int)source->size()) {
            auto [token, i, newline] = findMatch(index, line);
            if (!(token->type == Type::Whitespace || (parenCount == 0 && token->type == Type::Newline))) {
                tokens.push_back(token);
            }
            index = i;
            line = newline;
        }
        tokens.push_back(std::make_shared<Token>(Token{Type::EndOfFile, "EOF"}));
        return &tokens;
    }
}