#include "lexer.h"
#include <regex>

using namespace std;

std::tuple<Token, int> findMatch(std::string* line, int index) {
    smatch match;
    string sub = line->substr(index);
    Token token = UNKNOWN;
    int endIndex = -1;
    if (regex_search(sub, match, re_newline) && match.position() == 0) { endIndex = index + 1; token = NEWLINE; } 
    else if (regex_search(sub, match, re_tab) && match.position() == 0) { endIndex = index + match.str().length(); token = TAB; }
    else if (regex_search(sub, match, re_if) && match.position() == 0) { endIndex = index + 2; token = IF; }
    else if (regex_search(sub, match, re_elif) && match.position() == 0) { endIndex = index + 4; token = ELIF; }
    else if (regex_search(sub, match, re_else) && match.position() == 0) { endIndex = index + 4; token = ELSE; }
    else if (regex_search(sub, match, re_def) && match.position() == 0) { endIndex = index + 3; token = DEF; }
    else if (regex_search(sub, match, re_print) && match.position() == 0) { endIndex = index + 5; token = PRINT; }
    else if (regex_search(sub, match, re_lpar) && match.position() == 0) { endIndex = index + 1; token = LPAR; }
    else if (regex_search(sub, match, re_rpar) && match.position() == 0) { endIndex = index + 1; token = RPAR; }
    else if (regex_search(sub, match, re_colon) && match.position() == 0) { endIndex = index + 1; token = COLON; }
    else if (regex_search(sub, match, re_add) && match.position() == 0) { endIndex = index + 1; token = ADD; }
    else if (regex_search(sub, match, re_sub) && match.position() == 0) { endIndex = index + 1; token = SUB; }
    else if (regex_search(sub, match, re_mul) && match.position() == 0) { endIndex = index + 1; token = MUL; }
    else if (regex_search(sub, match, re_ddiv) && match.position() == 0) { endIndex = index + 2; token = DDIV; }
    else if (regex_search(sub, match, re_div) && match.position() == 0) { endIndex = index + 1; token = DIV; }
    else if (regex_search(sub, match, re_equal) && match.position() == 0) { endIndex = index + 2; token = EQUAL; }
    else if (regex_search(sub, match, re_assign) && match.position() == 0) { endIndex = index + 1; token = ASSIGN; }
    else if (regex_search(sub, match, re_greater_equal) && match.position() == 0) { endIndex = index + 2; token = GREATER_EQUAL; }
    else if (regex_search(sub, match, re_greater) && match.position() == 0) { endIndex = index + 1; token = GREATER; }
    else if (regex_search(sub, match, re_less_equal) && match.position() == 0) { endIndex = index + 2; token = LESS_EQUAL; }
    else if (regex_search(sub, match, re_less) && match.position() == 0) { endIndex = index + 1; token = LESS; }
    else if (regex_search(sub, match, re_or) && match.position() == 0) { endIndex = index + 2; token = OR; }
    else if (regex_search(sub, match, re_and) && match.position() == 0) { endIndex = index + 3; token = AND; }
    else if (regex_search(sub, match, re_true) && match.position() == 0) { endIndex = index + 4; token = TRUE; }
    else if (regex_search(sub, match, re_false) && match.position() == 0) { endIndex = index + 5; token = FALSE; }
    else if (regex_search(sub, match, re_none) && match.position() == 0) { endIndex = index + 4; token = NONE; }
    else if (regex_search(sub, match, re_return) && match.position() == 0) { endIndex = index + 6; token = RETURN; }
    else if (regex_search(sub, match, re_continue) && match.position() == 0) { endIndex = index + 8; token = CONTINUE; }
    else if (regex_search(sub, match, re_break) && match.position() == 0) { endIndex = index + 5; token = BREAK; }
    else if (regex_search(sub, match, re_for) && match.position() == 0) { endIndex = index + 3; token = FOR; }
    else if (regex_search(sub, match, re_while) && match.position() == 0) { endIndex = index + 5; token = WHILE; }
    else if (regex_search(sub, match, re_id) && match.position() == 0) { endIndex = index + match.str().length(); token = ID; }
    else if (regex_search(sub, match, re_ws) && match.position() == 0) { endIndex = index + match.str().length(); token = WS; }
    
    return {token, endIndex};
}

void printTokens(std::vector<Token> tokens) {
    int i = 0;
    
    for(auto t : tokens) {
        if(t == TAB) { std::cout << "TAB"; }
        else if(t == NEWLINE) { std::cout << "NEWLINE"; }
        else if(t == IF) { std::cout << "IF"; }
        else if(t == ELIF) { std::cout << "ELIF"; }
        else if(t == ELSE) { std::cout << "ELSE"; }
        else if(t == DEF) { std::cout << "DEF"; }
        else if(t == PRINT) { std::cout << "PRINT"; }
        else if(t == LPAR) { std::cout << "LPAR"; }
        else if(t == RPAR) { std::cout << "RPAR"; }
        else if(t == COLON) { std::cout << "COLON"; }
        else if(t == ADD) { std::cout << "ADD"; }
        else if(t == SUB) { std::cout << "SUB"; }
        else if(t == MUL) { std::cout << "MUL"; }
        else if(t == DDIV) { std::cout << "DDIV"; }
        else if(t == DIV) { std::cout << "DIV"; }
        else if(t == EQUAL) { std::cout << "EQUAL"; }
        else if(t == ASSIGN) { std::cout << "ASSIGN"; }
        else if(t == GREATER_EQUAL) { std::cout << "GREATER_EQUAL"; }
        else if(t == GREATER) { std::cout << "GREATER"; }
        else if(t == LESS_EQUAL) { std::cout << "LESS_EQUAL"; }
        else if(t == LESS) { std::cout << "LESS"; }
        else if(t == NOT_EQUAL) { std::cout << "NOT_EQUAL"; }
        else if(t == NOT) { std::cout << "NOT"; }
        else if(t == OR) { std::cout << "OR"; }
        else if(t == AND) { std::cout << "AND"; }
        else if(t == TRUE) { std::cout << "TRUE"; }
        else if(t == FALSE) { std::cout << "FALSE"; }
        else if(t == NONE) { std::cout << "NONE"; }
        else if(t == RETURN) { std::cout << "RETURN"; }
        else if(t == CONTINUE) { std::cout << "CONTINUE"; }
        else if(t == BREAK) { std::cout << "BREAK"; }
        else if(t == FOR) { std::cout << "FOR"; }
        else if(t == WHILE) { std::cout << "WHILE"; }
        else if(t == ID) { std::cout << "ID"; }
        else if(t == ID) { std::cout << "WS"; }
        else { std::cout << "UNKNOWN"; }

        if(i < tokens.size()-1) {
            std::cout << ", ";
        }
        i++;
    }
    std::cout << std::endl;
}

std::vector<Token> tokenize(std::string* source) {
    vector<Token> tokens;
    int index = 0;
    while(index < source->size()) {
        auto [token, i] = findMatch(source, index);
        tokens.push_back(token);
        index = i;
    }
    printTokens(tokens);
    return tokens;
}