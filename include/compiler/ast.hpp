#include <compiler/token.hpp>
#include <vector>
#include <fmt/format.h>
/*
struct Node
{
	Node* par;
	std::vector<Node*> children;
	Token data;

	Node& operator=(const Node& other) {
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
};


template <>
struct fmt::formatter<Node> : fmt::formatter<std::string> {

	// 格式化函数
	template <typename FormatContext>
	auto format(const Node& node, FormatContext& ctx) const {
		// 这里我们直接调用一个辅助函数来递归打印树
		return format_node(node, 0, ctx);
	}

private:
	// 辅助递归函数：depth 用于控制缩进
	template <typename FormatContext>
	auto format_node(const Node& node, int depth, FormatContext& ctx) const {
		// 1. 输出缩进和当前节点的值
		auto out = ctx.out();
		out = fmt::format_to(out, "{:{}}{}\n", "", depth * 2, node.data.lexeme);

		// 2. 递归格式化所有子节点
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
	fmt::print("AST Tree：{}", *node);
}

/*
class AstGenerator {
private:
	std::vector<Token> tokens;
	size_t pos;

	// 辅助方法：获取当前 Token
	Token current() const {
		return (pos < tokens.size()) ? tokens[pos] : Token{ TokenType::EndOfFile, "" };
	}

	// 辅助方法：消耗当前 Token 并前进
	Token consume() {
		Token tok = current();
		if (pos < tokens.size()) ++pos;
		return tok;
	}

	// 辅助方法：匹配并消耗特定类型的 Token
	Token expect(TokenType type) {
		if (current().type == type) return consume();
		throw std::runtime_error("Syntax Error: Unexpected token at pos " + std::to_string(pos));
	}

	// 递归下降规则：解析表达式（处理加减法，低优先级）
	Node* parseExpression() {
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
	Node* parseTerm() {
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
	Node* parseFactor() {
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

public:
	// 构造函数
	explicit AstGenerator(std::vector<Token> tokens_vec)
		: tokens(std::move(tokens_vec)), pos(0) {
	}

	// 析构函数：递归清理整棵 AST 树，防止内存泄漏
	~AstGenerator() = default;

	// 对外暴露的入口函数
	Node* geneAstTree() {
		if (tokens.empty()) return nullptr;
		return parseExpression();
	}

	// 递归销毁 AST 树的辅助方法
	static void destroyTree(Node* node) {
		if (!node) return;
		for (Node* child : node->children) {
			destroyTree(child);
		}
		delete node;
	}
};

*/


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
		// 这里我们直接调用一个辅助函数来递归打印树
		return format_node(node, 0, ctx);
	}

private:
	// 辅助递归函数：depth 用于控制缩进
	template <typename FormatContext>
	auto format_node(const Node& node, int depth, FormatContext& ctx) const {
		// 1. 输出缩进和当前节点的值
		auto out = ctx.out();
		out = fmt::format_to(out, "{:{}}{}\n", "", depth * 2, node.data.lexeme);

		// 2. 递归格式化所有子节点
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
	fmt::print("AST Tree：{}", *node);
}


class AstGenerator {
private:
	std::vector<Token> tokens;
	size_t pos;


	Token current() const;

	// 辅助方法：消耗当前 Token 并前进
	Token consume();

	// 辅助方法：匹配并消耗特定类型的 Token
	Token expect(TokenType type);

	// 递归下降规则：解析表达式（处理加减法，低优先级）
	Node* parseExpression();


	// 递归下降规则：解析项（处理乘除法，高优先级）
	Node* parseTerm();

	// 递归下降规则：解析因子（处理数字、标识符、括号）
	Node* parseFactor();

public:
	// 构造函数
	explicit AstGenerator(std::vector<Token> tokens_vec)
		: tokens(std::move(tokens_vec)), pos(0) {
	}

	// 析构函数：递归清理整棵 AST 树，防止内存泄漏
	~AstGenerator() = default;

	// 对外暴露的入口函数
	Node* geneAstTree();

	// 递归销毁 AST 树的辅助方法
	static void destroyTree(Node* node) {
		if (!node) return;
		for (Node* child : node->children) {
			destroyTree(child);
		}
		delete node;
	}
};
