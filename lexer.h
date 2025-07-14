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
    inline std::regex re_mod(R"(^%)");
    inline std::regex re_plus(R"(^\+)");
    inline std::regex re_minus(R"(^-)");
    inline std::regex re_dstar(R"(^\*\*)");
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
    inline std::regex re_is(R"(^is)");

    // literals
    inline std::regex re_float(R"(^\d+.\d+)");
    inline std::regex re_int(R"(^\d+)");
    inline std::regex re_string(R"(^\".*\")");
    inline std::regex re_true(R"(^True)");
    inline std::regex re_false(R"(^False)");
    inline std::regex re_none(R"(^None)");
    inline std::regex re_id(R"(^[a-zA-Z][a-zA-Z0-9]*)");

    enum class Type {
        // whitespace
        Comment, Tab, Newline, Whitespace,

        // operations
        BitOr, BitAnd, ShRight, ShLeft, Xor, Negate,
        DSlash, Slash, Mod, Plus, Minus, DStar, Star, Colon, Period, Comma, 
        
        // parens
        LPar, RPar, LSquare, RSquare, LBrace, RBrace,
        
        // comparisons
        GreaterEqual, Greater, LessEqual, Less, NotEqual, Equal, Assign,

        // keywords

        // TODO:
        // add class keyword
        // add import, from, as keywords
        // **, += -= *= /= %= :=

        If, Elif, Else, Def, Or, Not, And, Return, Continue, Break,
        For, While, In, Is, Pass, Arrow, 

        // literals and variables
        // id can be a variable name, function name, or type
        // which of these it is will be inferenced in the parser
        Id, Float, Int, String, True, False, None,

        // other
        EndOfFile,
        Invalid
    };

    using Literal = std::variant<std::monostate, std::string, int, double, bool>;

    struct Token {
        Type type = Type::Invalid;
        std::string lexeme = "NONE";
        Literal literal = std::monostate();
        int line = 0;
    };
    
    class Lexer {
    public:
        Lexer(std::string *source);
        std::tuple<Token, size_t, int> findMatch(size_t index, int line);
        void printTokens(bool advanced = false);
        std::vector<Token> *tokenize();
    private:
        size_t parenCount = 0;
        std::string *source;
        std::vector<Token> tokens;
    };
}