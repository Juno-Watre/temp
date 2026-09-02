#pragma once

#include <compiler/token.hpp>
#include <vector>


struct Node
{
    Node* par;
    std::vector<Node*> children;
    Token data;

    Node& operator=(const Node& other);
};


class AstGenerator {
private:
    std::vector<Token> tokens;
    size_t pos{};


    [[nodiscard]] Token current() const;

    // advance and return current Token
    Token consume();

    // match and consume a Token of the expected type
    Token expect(TokenType type);

    // recursive descent: parse expression (addition/subtraction, lowest precedence)
    Node* parseExpression();


    // recursive descent: parse term (multiplication/division, higher precedence)
    Node* parseTerm();

    // recursive descent: parse factor (number, identifier, parentheses)
    Node* parseFactor();

public:
    // constructor
    explicit AstGenerator(std::vector<Token> tokens_vec)
        : tokens(std::move(tokens_vec)){
    }

    // destructor — recursively builds AST; caller must prevent memory leaks
    ~AstGenerator() = default;

    // public entry point
    Node* geneAstTree();

    // recursively destroy AST tree to free memory
    static void destroyTree(Node* node) {
        if (node == nullptr) { 
            return;
        }
        for (Node* child : node->children) {
            destroyTree(child);
        }
        delete node;
    }
};