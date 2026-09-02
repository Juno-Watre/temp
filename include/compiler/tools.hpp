#include "compiler/ast.hpp"
#include <compiler/lexer.hpp>
#include <map>
#include <string>
#include <vector>

const std::string SPACE = "    ";
const std::vector<std::map<std::string, std::string>> TYPE_NAME = {
    {},
};

class ToolsClass {
public:
    static void display(Node* root);
};

