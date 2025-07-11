#include <regex>
#include <iostream>
#include "lexer.h"

std::tuple<Lexer::Token, int, int> Lexer::findMatch(std::string* source, int index, int line) {
    std::smatch match;
    std::string sub = source->substr(index);
    Lexer::Token token = {
        .type = Lexer::TokenType::Invalid,
        .lexeme = "NONE",
        .literal = std::monostate(),
        .line = line,
    };
    int endIndex = -1;
    // Only reassign token.type and token.literal
    if (regex_search(sub, match, re_comment)) { token.type=Lexer::TokenType::Comment; }
    else if (regex_search(sub, match, re_tab)) { token.type=Lexer::TokenType::Tab; }
    else if (regex_search(sub, match, re_newline)) { line += 1; token.type=Lexer::TokenType::Newline; }
    else if (regex_search(sub, match, re_ws)) { token.type=Lexer::TokenType::Whitespace; }
    else if (regex_search(sub, match, re_arrow)) { token.type=Lexer::TokenType::Arrow; }
    else if (regex_search(sub, match, re_bitor)) { token.type=Lexer::TokenType::BitOr; }
    else if (regex_search(sub, match, re_bitand)) { token.type=Lexer::TokenType::BitAnd; }
    else if (regex_search(sub, match, re_shright)) { token.type=Lexer::TokenType::ShRight; }
    else if (regex_search(sub, match, re_shleft)) { token.type=Lexer::TokenType::ShLeft; }
    else if (regex_search(sub, match, re_negate)) { token.type=Lexer::TokenType::Negate; }
    else if (regex_search(sub, match, re_xor)) { token.type=Lexer::TokenType::Xor; }
    else if (regex_search(sub, match, re_dslash)) { token.type=Lexer::TokenType::DSlash; }
    else if (regex_search(sub, match, re_slash)) { token.type=Lexer::TokenType::Slash; }
    else if (regex_search(sub, match, re_plus)) { token.type=Lexer::TokenType::Plus; }
    else if (regex_search(sub, match, re_minus)) { token.type=Lexer::TokenType::Minus; }
    else if (regex_search(sub, match, re_star)) { token.type=Lexer::TokenType::Star; }
    else if (regex_search(sub, match, re_colon)) { token.type=Lexer::TokenType::Colon; }
    else if (regex_search(sub, match, re_comma)) { token.type=Lexer::TokenType::Comma; }
    else if (regex_search(sub, match, re_period)) { token.type=Lexer::TokenType::Period; }
    else if (regex_search(sub, match, re_lpar)) { token.type=Lexer::TokenType::LPar; }
    else if (regex_search(sub, match, re_rpar)) { token.type=Lexer::TokenType::RPar; }
    else if (regex_search(sub, match, re_lsquare)) { token.type=Lexer::TokenType::LSquare; }
    else if (regex_search(sub, match, re_rsquare)) { token.type=Lexer::TokenType::RSquare; }
    else if (regex_search(sub, match, re_lbrace)) { token.type=Lexer::TokenType::LBrace; }
    else if (regex_search(sub, match, re_rbrace)) { token.type=Lexer::TokenType::RBrace; }
    else if (regex_search(sub, match, re_greater_equal)) { token.type=Lexer::TokenType::GreaterEqual; }
    else if (regex_search(sub, match, re_greater)) { token.type=Lexer::TokenType::Greater; }
    else if (regex_search(sub, match, re_less_equal)) { token.type=Lexer::TokenType::LessEqual; }
    else if (regex_search(sub, match, re_less)) { token.type=Lexer::TokenType::Less; }
    else if (regex_search(sub, match, re_not_equal)) { token.type=Lexer::TokenType::NotEqual; }
    else if (regex_search(sub, match, re_equal)) { token.type=Lexer::TokenType::Equal; }
    else if (regex_search(sub, match, re_assign)) { token.type=Lexer::TokenType::Assign; }
    else if (regex_search(sub, match, re_if)) { token.type=Lexer::TokenType::If; }
    else if (regex_search(sub, match, re_elif)) { token.type=Lexer::TokenType::Elif; }
    else if (regex_search(sub, match, re_else)) { token.type=Lexer::TokenType::Else; }
    else if (regex_search(sub, match, re_def)) { token.type=Lexer::TokenType::Def; }
    else if (regex_search(sub, match, re_or)) { token.type=Lexer::TokenType::Or; }
    else if (regex_search(sub, match, re_and)) { token.type=Lexer::TokenType::And; }
    else if (regex_search(sub, match, re_not)) { token.type=Lexer::TokenType::Not; }
    else if (regex_search(sub, match, re_return)) { token.type=Lexer::TokenType::Return; }
    else if (regex_search(sub, match, re_continue)) { token.type=Lexer::TokenType::Continue; }
    else if (regex_search(sub, match, re_break)) { token.type=Lexer::TokenType::Break; }
    else if (regex_search(sub, match, re_for)) { token.type=Lexer::TokenType::For; }
    else if (regex_search(sub, match, re_while)) { token.type=Lexer::TokenType::While; }
    else if (regex_search(sub, match, re_in)) { token.type=Lexer::TokenType::In; }
    else if (regex_search(sub, match, re_float)) { token.type=Lexer::TokenType::Float; token.literal=std::stod(match.str());}
    else if (regex_search(sub, match, re_int)) { token.type=Lexer::TokenType::Int; token.literal=std::stoi(match.str());}
    else if (regex_search(sub, match, re_string)) { token.type=Lexer::TokenType::String; token.literal=match.str();}
    else if (regex_search(sub, match, re_true)) { token.type=Lexer::TokenType::True; token.literal=true;}
    else if (regex_search(sub, match, re_false)) { token.type=Lexer::TokenType::False; token.literal=false;}
    else if (regex_search(sub, match, re_none)) { token.type=Lexer::TokenType::None; token.literal=std::monostate();}
    else if (regex_search(sub, match, re_id)) { token.type=Lexer::TokenType::Id; token.literal=match.str();}
    endIndex = index + match.str().length();
    token.lexeme=match.str();
    return {token, endIndex, line};
}

void Lexer::printTokens(std::vector<Token> tokens, bool advanced) {
    int i = 0;
    
    for(auto t : tokens) {
        if(t.type == Lexer::TokenType::Comment) { std::cout << "Comment"; }
        else if(t.type == Lexer::TokenType::Tab) { std::cout << "Tab"; }
        else if(t.type == Lexer::TokenType::Newline) { std::cout << "Newline" << std::endl; }
        else if(t.type == Lexer::TokenType::Whitespace) { std::cout << "Whitespace"; }
        else if(t.type == Lexer::TokenType::Arrow) { std::cout << "Arrow"; }
        else if(t.type == Lexer::TokenType::BitOr) { std::cout << "BitOr"; }
        else if(t.type == Lexer::TokenType::BitAnd) { std::cout << "BitAnd"; }
        else if(t.type == Lexer::TokenType::ShRight) { std::cout << "ShRight"; }
        else if(t.type == Lexer::TokenType::ShLeft) { std::cout << "ShLeft"; }
        else if(t.type == Lexer::TokenType::Negate) { std::cout << "Negate"; }
        else if(t.type == Lexer::TokenType::Xor) { std::cout << "Xor"; }
        else if(t.type == Lexer::TokenType::DSlash) { std::cout << "DSlash"; }
        else if(t.type == Lexer::TokenType::Slash) { std::cout << "Slash"; }
        else if(t.type == Lexer::TokenType::Plus) { std::cout << "Plus"; }
        else if(t.type == Lexer::TokenType::Minus) { std::cout << "Minus"; }
        else if(t.type == Lexer::TokenType::Star) { std::cout << "Star"; }
        else if(t.type == Lexer::TokenType::Colon) { std::cout << "Colon"; }
        else if(t.type == Lexer::TokenType::Comma) { std::cout << "Comma"; }
        else if(t.type == Lexer::TokenType::Period) { std::cout << "Period"; }   
        else if(t.type == Lexer::TokenType::LPar) { std::cout << "LPar"; }
        else if(t.type == Lexer::TokenType::RPar) { std::cout << "RPar"; }
        else if(t.type == Lexer::TokenType::LSquare) { std::cout << "LSquare"; }
        else if(t.type == Lexer::TokenType::RSquare) { std::cout << "RSquare"; }
        else if(t.type == Lexer::TokenType::LBrace) { std::cout << "LBrace"; }
        else if(t.type == Lexer::TokenType::RBrace) { std::cout << "RBrace"; }
        else if(t.type == Lexer::TokenType::GreaterEqual) { std::cout << "GreaterEqual"; }
        else if(t.type == Lexer::TokenType::Greater) { std::cout << "Greater"; }
        else if(t.type == Lexer::TokenType::LessEqual) { std::cout << "LessEqual"; }
        else if(t.type == Lexer::TokenType::Less) { std::cout << "Less"; }
        else if(t.type == Lexer::TokenType::NotEqual) { std::cout << "NotEqual"; }
        else if(t.type == Lexer::TokenType::Equal) { std::cout << "Equal"; }
        else if(t.type == Lexer::TokenType::Assign) { std::cout << "Assign"; }

        else if(t.type == Lexer::TokenType::If) { std::cout << "If"; }
        else if(t.type == Lexer::TokenType::Elif) { std::cout << "Elif"; }
        else if(t.type == Lexer::TokenType::Else) { std::cout << "Else"; }
        else if(t.type == Lexer::TokenType::Def) { std::cout << "Def"; }
        else if(t.type == Lexer::TokenType::Or) { std::cout << "Or"; }
        else if(t.type == Lexer::TokenType::And) { std::cout << "And"; }
        else if(t.type == Lexer::TokenType::Not) { std::cout << "Not"; }
        else if(t.type == Lexer::TokenType::Return) { std::cout << "Return"; }
        else if(t.type == Lexer::TokenType::Continue) { std::cout << "Continue"; }
        else if(t.type == Lexer::TokenType::Break) { std::cout << "Br"; }
        else if(t.type == Lexer::TokenType::For) { std::cout << "For"; }
        else if(t.type == Lexer::TokenType::While) { std::cout << "While"; }
        else if(t.type == Lexer::TokenType::In) { std::cout << "In"; }

        else if(t.type == Lexer::TokenType::Float) { std::cout << "Float"; }
        else if(t.type == Lexer::TokenType::Int) { std::cout << "Int"; }
        else if(t.type == Lexer::TokenType::String) { std::cout << "String"; }
        else if(t.type == Lexer::TokenType::True) { std::cout << "True"; }
        else if(t.type == Lexer::TokenType::False) { std::cout << "False"; }
        else if(t.type == Lexer::TokenType::None) { std::cout << "None"; }
        else if(t.type == Lexer::TokenType::Id) { std::cout << "Id"; }
        else { std::cout << "INVALID"; }
        
        if(i < tokens.size()-1) { std::cout << ", "; }
        ++i;
    }
    i = 0;
    if (advanced) {
        std::cout << "\n" << std::endl;
        for(auto t : tokens) {
            std::cout << t.lexeme;
            if(i < tokens.size()-1) { std::cout << ", "; }
            ++i;
        }
    }
    std::cout << std::endl;
}

std::vector<Lexer::Token> Lexer::tokenize(std::string* source) {
    std::vector<Token> tokens;
    int index = 0;
    int line = 0;
    while(index != -1 && index < source->size()) {
        auto [token, i, newline] = findMatch(source, index, line);
        if (token.type!=Lexer::TokenType::Whitespace) {
            tokens.push_back(token);
        }
        index = i;
        line = newline;
    }
    printTokens(tokens, true);
    return tokens;
}