#include <regex>
#include <iostream>
#include "lexer.h"

namespace Lexer {
    Lexer::Lexer(std::string *source) : source(source) {};

    std::tuple<Token, size_t, int> Lexer::findMatch(size_t index, int line) {
        std::smatch match;
        std::string sub = source->substr(index);
        Token token;
        size_t newIndex = -1;
        // Only reassign token.type and token.literal
        if (regex_search(sub, match, re_comment)) { token.type=TokenType::Comment; }
        else if (regex_search(sub, match, re_tab)) { token.type=TokenType::Tab; }
        else if (regex_search(sub, match, re_newline)) { line += 1; token.type=TokenType::Newline; }
        else if (regex_search(sub, match, re_ws)) { token.type=TokenType::Whitespace; }
        else if (regex_search(sub, match, re_arrow)) { token.type=TokenType::Arrow; }
        else if (regex_search(sub, match, re_bitor)) { token.type=TokenType::BitOr; }
        else if (regex_search(sub, match, re_bitand)) { token.type=TokenType::BitAnd; }
        else if (regex_search(sub, match, re_shright)) { token.type=TokenType::ShRight; }
        else if (regex_search(sub, match, re_shleft)) { token.type=TokenType::ShLeft; }
        else if (regex_search(sub, match, re_negate)) { token.type=TokenType::Negate; }
        else if (regex_search(sub, match, re_xor)) { token.type=TokenType::Xor; }
        else if (regex_search(sub, match, re_dslash)) { token.type=TokenType::DSlash; }
        else if (regex_search(sub, match, re_slash)) { token.type=TokenType::Slash; }
        else if (regex_search(sub, match, re_plus)) { token.type=TokenType::Plus; }
        else if (regex_search(sub, match, re_minus)) { token.type=TokenType::Minus; }
        else if (regex_search(sub, match, re_star)) { token.type=TokenType::Star; }
        else if (regex_search(sub, match, re_colon)) { token.type=TokenType::Colon; }
        else if (regex_search(sub, match, re_comma)) { token.type=TokenType::Comma; }
        else if (regex_search(sub, match, re_period)) { token.type=TokenType::Period; }
        else if (regex_search(sub, match, re_lpar)) { token.type=TokenType::LPar; }
        else if (regex_search(sub, match, re_rpar)) { token.type=TokenType::RPar; }
        else if (regex_search(sub, match, re_lsquare)) { token.type=TokenType::LSquare; }
        else if (regex_search(sub, match, re_rsquare)) { token.type=TokenType::RSquare; }
        else if (regex_search(sub, match, re_lbrace)) { token.type=TokenType::LBrace; }
        else if (regex_search(sub, match, re_rbrace)) { token.type=TokenType::RBrace; }
        else if (regex_search(sub, match, re_greater_equal)) { token.type=TokenType::GreaterEqual; }
        else if (regex_search(sub, match, re_greater)) { token.type=TokenType::Greater; }
        else if (regex_search(sub, match, re_less_equal)) { token.type=TokenType::LessEqual; }
        else if (regex_search(sub, match, re_less)) { token.type=TokenType::Less; }
        else if (regex_search(sub, match, re_not_equal)) { token.type=TokenType::NotEqual; }
        else if (regex_search(sub, match, re_equal)) { token.type=TokenType::Equal; }
        else if (regex_search(sub, match, re_assign)) { token.type=TokenType::Assign; }
        else if (regex_search(sub, match, re_if)) { token.type=TokenType::If; }
        else if (regex_search(sub, match, re_elif)) { token.type=TokenType::Elif; }
        else if (regex_search(sub, match, re_else)) { token.type=TokenType::Else; }
        else if (regex_search(sub, match, re_def)) { token.type=TokenType::Def; }
        else if (regex_search(sub, match, re_or)) { token.type=TokenType::Or; }
        else if (regex_search(sub, match, re_and)) { token.type=TokenType::And; }
        else if (regex_search(sub, match, re_not)) { token.type=TokenType::Not; }
        else if (regex_search(sub, match, re_return)) { token.type=TokenType::Return; }
        else if (regex_search(sub, match, re_continue)) { token.type=TokenType::Continue; }
        else if (regex_search(sub, match, re_break)) { token.type=TokenType::Break; }
        else if (regex_search(sub, match, re_for)) { token.type=TokenType::For; }
        else if (regex_search(sub, match, re_while)) { token.type=TokenType::While; }
        else if (regex_search(sub, match, re_in)) { token.type=TokenType::In; }
        else if (regex_search(sub, match, re_float)) { token.type=TokenType::Float; token.literal=std::stod(match.str());}
        else if (regex_search(sub, match, re_int)) { token.type=TokenType::Int; token.literal=std::stoi(match.str());}
        else if (regex_search(sub, match, re_string)) { token.type=TokenType::String; token.literal=match.str();}
        else if (regex_search(sub, match, re_true)) { token.type=TokenType::True; token.literal=true;}
        else if (regex_search(sub, match, re_false)) { token.type=TokenType::False; token.literal=false;}
        else if (regex_search(sub, match, re_none)) { token.type=TokenType::None; token.literal=std::monostate();}
        else if (regex_search(sub, match, re_id)) { token.type=TokenType::Id; token.literal=match.str();}
        newIndex = index + match.str().length();
        token.lexeme=match.str();
        return {token, newIndex, line};
    }

    void Lexer::printTokens(bool advanced) {
        size_t i = 0;
        
        for(auto t : tokens) {
            if(t.type == TokenType::Comment) { std::cout << "Comment"; }
            else if(t.type == TokenType::Tab) { std::cout << "Tab"; }
            else if(t.type == TokenType::Newline) { std::cout << "Newline"; }
            else if(t.type == TokenType::Whitespace) { std::cout << "Whitespace"; }
            else if(t.type == TokenType::Arrow) { std::cout << "Arrow"; }
            else if(t.type == TokenType::BitOr) { std::cout << "BitOr"; }
            else if(t.type == TokenType::BitAnd) { std::cout << "BitAnd"; }
            else if(t.type == TokenType::ShRight) { std::cout << "ShRight"; }
            else if(t.type == TokenType::ShLeft) { std::cout << "ShLeft"; }
            else if(t.type == TokenType::Negate) { std::cout << "Negate"; }
            else if(t.type == TokenType::Xor) { std::cout << "Xor"; }
            else if(t.type == TokenType::DSlash) { std::cout << "DSlash"; }
            else if(t.type == TokenType::Slash) { std::cout << "Slash"; }
            else if(t.type == TokenType::Plus) { std::cout << "Plus"; }
            else if(t.type == TokenType::Minus) { std::cout << "Minus"; }
            else if(t.type == TokenType::Star) { std::cout << "Star"; }
            else if(t.type == TokenType::Colon) { std::cout << "Colon"; }
            else if(t.type == TokenType::Comma) { std::cout << "Comma"; }
            else if(t.type == TokenType::Period) { std::cout << "Period"; }   
            else if(t.type == TokenType::LPar) { std::cout << "LPar"; }
            else if(t.type == TokenType::RPar) { std::cout << "RPar"; }
            else if(t.type == TokenType::LSquare) { std::cout << "LSquare"; }
            else if(t.type == TokenType::RSquare) { std::cout << "RSquare"; }
            else if(t.type == TokenType::LBrace) { std::cout << "LBrace"; }
            else if(t.type == TokenType::RBrace) { std::cout << "RBrace"; }
            else if(t.type == TokenType::GreaterEqual) { std::cout << "GreaterEqual"; }
            else if(t.type == TokenType::Greater) { std::cout << "Greater"; }
            else if(t.type == TokenType::LessEqual) { std::cout << "LessEqual"; }
            else if(t.type == TokenType::Less) { std::cout << "Less"; }
            else if(t.type == TokenType::NotEqual) { std::cout << "NotEqual"; }
            else if(t.type == TokenType::Equal) { std::cout << "Equal"; }
            else if(t.type == TokenType::Assign) { std::cout << "Assign"; }

            else if(t.type == TokenType::If) { std::cout << "If"; }
            else if(t.type == TokenType::Elif) { std::cout << "Elif"; }
            else if(t.type == TokenType::Else) { std::cout << "Else"; }
            else if(t.type == TokenType::Def) { std::cout << "Def"; }
            else if(t.type == TokenType::Or) { std::cout << "Or"; }
            else if(t.type == TokenType::And) { std::cout << "And"; }
            else if(t.type == TokenType::Not) { std::cout << "Not"; }
            else if(t.type == TokenType::Return) { std::cout << "Return"; }
            else if(t.type == TokenType::Continue) { std::cout << "Continue"; }
            else if(t.type == TokenType::Break) { std::cout << "Br"; }
            else if(t.type == TokenType::For) { std::cout << "For"; }
            else if(t.type == TokenType::While) { std::cout << "While"; }
            else if(t.type == TokenType::In) { std::cout << "In"; }

            else if(t.type == TokenType::Float) { std::cout << "Float"; }
            else if(t.type == TokenType::Int) { std::cout << "Int"; }
            else if(t.type == TokenType::String) { std::cout << "String"; }
            else if(t.type == TokenType::True) { std::cout << "True"; }
            else if(t.type == TokenType::False) { std::cout << "False"; }
            else if(t.type == TokenType::None) { std::cout << "None"; }
            else if(t.type == TokenType::Id) { std::cout << "Id"; }
            else if(t.type == TokenType::EndOfFile) { std::cout << "EndOfFile"; }
            else { std::cout << "INVALID"; }
            
            if(i < tokens.size()-1) { std::cout << ", "; }
            if(t.type == TokenType::Newline) { std::cout << std::endl; }
            ++i;
        }
        i = 0;
        if (advanced) {
            std::cout << "\n" << std::endl;
            for(auto t : tokens) {
                std::cout << t.lexeme;
                if(i < tokens.size()-1 && t.type != TokenType::Newline) { std::cout << ", "; }
                ++i;
            }
        }
        std::cout << std::endl;
    }

    std::vector<Token> *Lexer::tokenize() {
        size_t index = 0;
        int line = 0;
        while(index != -1 && index < source->size()) {
            auto [token, i, newline] = findMatch(index, line);
            if (token.type!=TokenType::Whitespace) {
                tokens.push_back(token);
            }
            index = i;
            line = newline;
        }
        tokens.push_back({TokenType::EndOfFile, "EOF"});
        return &tokens;
    }
}