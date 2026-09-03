#include <compiler/lexer.hpp>
#include <compiler/ast-builder.hpp>
#include <string>

const std::string SPACE = "    ";

class ToolsClass {
public:
    static void display(AstNode* root);
    static std::string_view tokenTypeToString(TokenType type);
};

