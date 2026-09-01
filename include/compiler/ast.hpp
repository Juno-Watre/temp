#pragma once

#include <compiler/token.hpp>
#include <vector>
#include <fmt/format.h>


struct Node
{
    Node* par;
    std::vector<Node*> children;
    Token data;

    Node& operator=(const Node& other);
};


template <>
struct fmt::formatter<Node> : fmt::formatter<std::string> {
    template <typename FormatContext>
    auto format(const Node& node, FormatContext& ctx) const {
        // delegate to recursive helper for indented output
        return format_node(node, 0, ctx);
    }

private:
    // recursive helper function, depth controls indentation
    template <typename FormatContext>
    auto format_node(const Node& node, int depth, FormatContext& ctx) const -> typename FormatContext::iterator {
        // 1. print indentation and current node value
        auto out = ctx.out();
        out = fmt::format_to(out, "{:{}}{}\n", "", depth * 2, node.data.lexeme);

        // 2. recursively format child nodes
        for (const auto& child : node.children) {
            if (child) {
                out = format_node(*child, depth + 1, ctx);
            }
        }
        return out;
    }
};

void
display(Node* node)
{
    fmt::print("AST Tree:{}", *node);
}


class AstGenerator {
private:
    std::vector<Token> tokens;
    size_t pos;


    Token current() const;

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
        : tokens(std::move(tokens_vec)), pos(0) {
    }

    // destructor — recursively builds AST; caller must prevent memory leaks
    ~AstGenerator() = default;

    // public entry point
    Node* geneAstTree();

    // recursively destroy AST tree to free memory
    static void destroyTree(Node* node) {
        if (!node) return;
        for (Node* child : node->children) {
            destroyTree(child);
        }
        delete node;
    }
};