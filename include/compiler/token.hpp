#include <string>

enum class TokenType
{
    Keyword,      // 关键字：if, while, return...
    Identifier,   // 标识符：变量名、函数名
    Integer,      // 整数
    Float,        // 浮点数
    String,       // 字符串
    Operator,     // 运算符：+, -, *, /
    Bracket,      // 括号：(, ), {, }
    Punctuation,  // 标点：, ; .
    Comment,      // 注释
    EndOfFile,    // 文件结束
    Error,         // 错误
};

struct Token
{
    TokenType type;       // 类型
    std::string lexeme;   // 原始文本
    size_t line;          // 行号
    size_t column;        // 列号

    Token& operator= (const Token& other) {
        type = other.type;
        column = other.column;
        line = other.line;
        lexeme = other.lexeme;
        return *this;
    }
};