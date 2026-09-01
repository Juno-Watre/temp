#pragma once

#include <string>

enum class TokenType
{
    Keyword,      // keyword: if, while, return...
    Identifier,   // identifier: variable names, function names
    Integer,      // integer literal
    Float,        // float literal
    String,       // string literal
    Operator,     // operator: +, -, *, /
    Bracket,      // bracket: (, ), {, }
    Punctuation,  // punctuation: , ; .
    Comment,      // comment
    EndOfFile,    // end of file
    Error,        // error token
};

struct Token
{
    TokenType type;       // token type
    std::string lexeme;   // raw text
    size_t line;          // line number
    size_t column;        // column number

    Token& operator= (const Token& other) {
        type = other.type;
        column = other.column;
        line = other.line;
        lexeme = other.lexeme;
        return *this;
    }
};