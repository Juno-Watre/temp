#include <compiler/ast.hpp>
#include <compiler/token.hpp>
#include <compiler/lexer.hpp>
#include <vector>
#include <fmt/format.h>
#include <stdexcept>

//AstGenerator::

// 辅助方法：获取当前 Token
Token AstGenerator::current() const {
	return (pos < tokens.size()) ? tokens[pos] : Token{ TokenType::EndOfFile, "" };
}

// 辅助方法：消耗当前 Token 并前进
Token AstGenerator::consume() {
	Token tok = current();
	if (pos < tokens.size()) ++pos;
	return tok;
}

// 辅助方法：匹配并消耗特定类型的 Token
Token AstGenerator::expect(TokenType type) {
	if (current().type == type) return consume();
	throw std::runtime_error("Syntax Error: Unexpected token at pos " + std::to_string(pos));
}

// 递归下降规则：解析表达式（处理加减法，低优先级）
Node* AstGenerator::parseExpression() {
	Node* left = parseTerm();
	while (current().lexeme == "-" || current().lexeme == "+") {
		Node* opNode = new Node();
		opNode->data = consume();
		opNode->children.push_back(left);
		opNode->children.push_back(parseTerm());
		left = opNode;
	}
	return left;
}

// 递归下降规则：解析项（处理乘除法，高优先级）
Node* AstGenerator::parseTerm() {
	Node* left = parseFactor();
	while (current().lexeme == "*" || current().lexeme == "/") {
		Node* opNode = new Node();
		opNode->data = consume();
		opNode->children.push_back(left);
		opNode->children.push_back(parseFactor());
		left = opNode;
	}
	return left;
}

// 递归下降规则：解析因子（处理数字、标识符、括号）
Node* AstGenerator::parseFactor() {
	Token tok = current();
	if (tok.type == TokenType::Integer || tok.type == TokenType::Identifier) {
		consume();
		Node* leaf = new Node();
		leaf->data = tok;
		return leaf;
	}
	else if (tok.lexeme == "(" || tok.lexeme == "{") {
		consume();
		Node* expr = parseExpression();
		expect(TokenType::Bracket);  // TODO :
		return expr;
	}
	throw std::runtime_error("Syntax Error: Expected number or '(' at pos " + std::to_string(pos));
}

// 对外暴露的入口函数
Node* AstGenerator::geneAstTree() {
	if (tokens.empty()) return nullptr;
	return parseExpression();
}





Node& Node::operator=(const Node& other) {
	if (this == &other) {
		return *this;
	}

	for (Node* child : children) {
		delete child;
	}
	children.clear();


	data = other.data;

	for (Node* child : other.children) {
		// 为每个子节点分配新内存，并递归调用拷贝构造或赋值
		Node* newChild = new Node();
		*newChild = *child; // 递归调用当前的 operator=
		children.push_back(newChild);
	}


	return *this;
}