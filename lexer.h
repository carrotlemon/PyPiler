#pragma once

#include <string>
#include <variant>
#include <optional>
#include <vector>
#include <regex>

namespace Lexer {
    // whitespace
    inline std::regex re_comment(R"(^#)");
    inline std::regex re_tab(R"(^(\t|    ))");
    inline std::regex re_newline(R"(^\n)");
    inline std::regex re_ws(R"(^ +)");

    // operations
    inline std::regex re_arrow(R"(^->)");
    inline std::regex re_bitor(R"(^\|)");
    inline std::regex re_bitand(R"(^&)");
    inline std::regex re_shright(R"(^>>)");
    inline std::regex re_shleft(R"(^<<)");
    inline std::regex re_negate(R"(^~)");
    inline std::regex re_xor(R"(^\^)");

    inline std::regex re_dslash(R"(^//)");
    inline std::regex re_slash(R"(^/)");
    inline std::regex re_plus(R"(^\+)");
    inline std::regex re_minus(R"(^-)");
    inline std::regex re_star(R"(^\*)");
    inline std::regex re_colon(R"(^:)");
    inline std::regex re_comma(R"(^,)");
    inline std::regex re_period(R"(^\.)");

    // paren
    inline std::regex re_lpar(R"(^\()");
    inline std::regex re_rpar(R"(^\))");
    inline std::regex re_lsquare(R"(^\[)");
    inline std::regex re_rsquare(R"(^\])");
    inline std::regex re_lbrace(R"(^\{)");
    inline std::regex re_rbrace(R"(^\})");

    // comparisons
    inline std::regex re_greater_equal(R"(^>=)");
    inline std::regex re_greater(R"(^>)");
    inline std::regex re_less_equal(R"(^<=)");
    inline std::regex re_less(R"(^<)");
    inline std::regex re_not_equal(R"(^!=)");
    inline std::regex re_equal(R"(^==)");
    inline std::regex re_assign(R"(^=)");

    // types
    // inline std::regex re_int_type(R"(^int)");
    // inline std::regex re_list_type(R"(^List)");
    // inline std::regex re_strtype(R"(^str)");
    // inline std::regex re_booltype(R"(^bool)");
    // inline std::regex re_floattype(R"(^float)");

    // keywords
    inline std::regex re_if(R"(^if)");
    inline std::regex re_elif(R"(^elif)");
    inline std::regex re_else(R"(^else)");
    inline std::regex re_def(R"(^def)");
    inline std::regex re_or(R"(^or)");
    inline std::regex re_and(R"(^and)");
    inline std::regex re_not(R"(^not)");
    inline std::regex re_return(R"(^return)");
    inline std::regex re_continue(R"(^continue)");
    inline std::regex re_break(R"(^break)");
    inline std::regex re_for(R"(^for)");
    inline std::regex re_while(R"(^while)");
    inline std::regex re_in(R"(^in)");

    // literals
    inline std::regex re_float(R"(^\d+.\d+)");
    inline std::regex re_int(R"(^\d+)");
    inline std::regex re_string(R"(^\".*\")");
    inline std::regex re_true(R"(^True)");
    inline std::regex re_false(R"(^False)");
    inline std::regex re_none(R"(^None)");
    inline std::regex re_id(R"(^[a-zA-Z][a-zA-Z0-9]*)");

    enum class TokenType {
        // whitespace
        Comment, Tab, Newline, Whitespace,

        // operations
        Arrow, BitOr, BitAnd, ShRight, ShLeft, Xor, Negate,
        DSlash, Slash, Plus, Minus, Star, Colon, Comma, Period,
        
        // paren
        LPar, RPar, LSquare, RSquare, LBrace, RBrace,
        
        // comparisons
        GreaterEqual, Greater, LessEqual, Less, NotEqual, Equal, Assign,

        // types
        // IntType, ListType, StrType, BoolType, FloatType,

        // keywords
        If, Elif, Else, Def, Or, Not, And, Return, Continue, Break,
        For, While, In, Pass,

        // literals and variables
        // id can be a variable name, function name, or type
        // which of these it is will be inferenced in the parser
        Id, Float, Int, String, True, False, None,

        // other
        EndOfFile,
        Invalid
    };

    struct Token {
        TokenType type;
        std::string lexeme;
        std::variant<std::monostate, std::string, int, double, bool> literal;
        int line;
    };
    
    std::tuple<Token, int, int> findMatch(std::string* source, int index, int line);
    void printTokens(std::vector<Token> tokens, bool advanced);
    std::vector<Token> tokenize(std::string* source);
}