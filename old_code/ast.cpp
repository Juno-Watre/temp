#include <compiler/ast.hpp>
#include <compiler/token.hpp>
#include <compiler/lexer.hpp>
#include <vector>
#include <stdexcept>


// Get current Token without advancing
Token AstGenerator::current() const {
    return (pos < tokens.size()) ? tokens[pos] : Token{ TokenType::EndOfFile, "" };
}

// Advance and return the current Token
Token AstGenerator::consume() {
    Token tok = current();
    if (pos < tokens.size()) ++pos;
    return tok;
}

// Match and consume a Token of the expected type
Token AstGenerator::expect(TokenType type) {
    if (current().type == type) return consume();
    throw std::runtime_error("Syntax Error: Unexpected token at pos " + std::to_string(pos));
}

// Recursive descent: parse expression (addition/subtraction, lowest precedence)
AstNode* AstGenerator::parseExpression() {
    AstNode* left = parseTerm();
    while (current().lexeme == "-" || current().lexeme == "+") {
        AstNode* opNode = new AstNode();
        opNode->data = consume();
        opNode->children.push_back(left);
        opNode->children.push_back(parseTerm());
        left = opNode;
    }
    return left;
}

// Recursive descent: parse term (multiplication/division, higher precedence)
AstNode* AstGenerator::parseTerm() {
    AstNode* left = parseFactor();
    while (current().lexeme == "*" || current().lexeme == "/") {
        AstNode* opNode = new AstNode();
        opNode->data = consume();
        opNode->children.push_back(left);
        opNode->children.push_back(parseFactor());
        left = opNode;
    }
    return left;
}

// Recursive descent: parse factor (number, identifier, parentheses)
AstNode* AstGenerator::parseFactor() {
    Token tok = current();
    if (tok.type == TokenType::Integer || tok.type == TokenType::Identifier) {
        consume();
        AstNode* leaf = new AstNode();
        leaf->data = tok;
        return leaf;
    }
    if (tok.type == TokenType::ParenOpen ) {
        consume();
        AstNode* expr = parseExpression();
        expect(TokenType::ParenClose);  // TODO : check matching bracket
        return expr;
    }
    if (tok.type == TokenType::BraceOpen ) {
        consume();
        AstNode* expr = parseExpression();
        expect(TokenType::BraceOpen);  // TODO : check matching bracket
        return expr;
    }
    throw std::runtime_error("Syntax Error: Expected number or '(' at pos " + std::to_string(pos));
}

// Public entry point
AstNode* AstGenerator::geneAstTree() {
    if (tokens.empty()) { 
        return nullptr;
    }
    return parseExpression();
}


AstNode& AstNode::operator=(const AstNode& other) {
    if (this == &other) {
        return *this;
    }

    for (AstNode* child : children) {
        delete child;
    }
    children.clear();


    data = other.data;

    // deep copy child nodes
    for (AstNode* child : other.children) {
        AstNode* newChild = new AstNode();
        *newChild = *child; // recursive call to operator=
        children.push_back(newChild);
    }

    return *this;
}