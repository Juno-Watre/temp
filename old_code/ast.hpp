#pragma once

#include <compiler/token.hpp>
#include <vector>


struct AstNode
{
    AstNode* par;
    std::vector<AstNode*> children;
    Token data;

    AstNode& operator=(const AstNode& other);
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
    AstNode* parseExpression();


    // recursive descent: parse term (multiplication/division, higher precedence)
    AstNode* parseTerm();

    // recursive descent: parse factor (number, identifier, parentheses)
    AstNode* parseFactor();

public:
    // constructor
    explicit AstGenerator(std::vector<Token> tokens_vec)
        : tokens(std::move(tokens_vec)){
    }

    // destructor — recursively builds AST; caller must prevent memory leaks
    ~AstGenerator() = default;

    // public entry point
    AstNode* geneAstTree();

    // recursively destroy AST tree to free memory
    static void destroyTree(AstNode* node) {
        if (node == nullptr) { 
            return;
        }
        for (AstNode* child : node->children) {
            destroyTree(child);
        }
        delete node;
    }
};

