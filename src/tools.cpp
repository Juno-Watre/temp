#include "compiler/ast.hpp"
#include <compiler/tools.hpp>
#include <queue>


// waiting for test
void ToolsClass::display(Node *root) {
    std::queue<Node*> nodes;
    int nests = 0;
    while (true) {
        if (root->children.size() == 0) {
            nests --;
            break;
        }
        for (Node* node:root ->children) {
            nodes.push(node);
            nests ++;
        }
        root = nodes.front();
        for (int i = 0; i < nests; i ++) {
            std::cout << SPACE;
        }
        std::cout << root->data.lexeme << '\n';
        nodes.pop();
    }
}