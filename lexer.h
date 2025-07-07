#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <regex>

inline std::regex re_newline(R"(^\n)");
inline std::regex re_tab(R"(^(\t|    ))");
inline std::regex re_if(R"(^if)");
inline std::regex re_elif(R"(^elif)");
inline std::regex re_else(R"(^else)");
inline std::regex re_def(R"(^def)");
inline std::regex re_print(R"(^print)");
inline std::regex re_lpar(R"(\()");
inline std::regex re_rpar(R"(\))");
inline std::regex re_colon(R"(^:)");
inline std::regex re_add(R"(\+)");
inline std::regex re_sub(R"(^-)");
inline std::regex re_mul(R"(\*)");
inline std::regex re_ddiv(R"(^//)");
inline std::regex re_div(R"(^/)");
inline std::regex re_equal(R"(==)");
inline std::regex re_assign(R"(=)");
inline std::regex re_greater_equal(R"(>=)");
inline std::regex re_greater(R"(>)");
inline std::regex re_less_equal(R"(<=)");
inline std::regex re_less(R"(<)");
inline std::regex re_or(R"(^or)");
inline std::regex re_and(R"(^and)");
inline std::regex re_true(R"(^True)");
inline std::regex re_false(R"(^False)");
inline std::regex re_none(R"(^None)");
inline std::regex re_return(R"(^return)");
inline std::regex re_continue(R"(^continue)");
inline std::regex re_break(R"(^break)");
inline std::regex re_for(R"(^for)");
inline std::regex re_while(R"(^while)");
inline std::regex re_id(R"(^[a-zA-Z][a-zA-Z0-9]*)");
inline std::regex re_ws(R"(^ +)");                 

enum Token {
    TAB,
    NEWLINE,
    
    IF,
    ELIF,
    ELSE,
    DEF,
    PRINT,
    LPAR,
    RPAR,

    COLON,
    ADD,
    SUB,
    MUL,
    DDIV,
    DIV,
    EQUAL,
    ASSIGN,
    
    GREATER_EQUAL,
    GREATER,
    LESS_EQUAL,
    LESS,
    NOT_EQUAL,
    NOT,
    OR,
    AND,
    
    TRUE,
    FALSE,
    NONE,

    RETURN,
    CONTINUE,
    BREAK,

    FOR,
    WHILE,

    ID,
    UNKNOWN,
    WS
    // ARROW
    // IN,
    // RANGE,
    // PASS
    // you can add bit operators
};

std::tuple<Token, int> findMatch(std::string* line, int index);
void printTokens(std::vector<Token> tokens);
std::vector<Token> tokenize(std::string* source);