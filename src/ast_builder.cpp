#include <compiler/ast-builder.hpp>

#include <iostream>
#include <functional>
#include <stdexcept>
#include <utility>

namespace {
bool trivia(TokenType t) { return t == TokenType::Whitespace || t == TokenType::Comment; }
bool open(TokenType t) { return t == TokenType::ParenOpen || t == TokenType::BraceOpen || t == TokenType::BracketOpen; }
bool close(TokenType t) { return t == TokenType::ParenClose || t == TokenType::BraceClose || t == TokenType::BracketClose; }
bool pair(TokenType a, TokenType b) {
    return (a == TokenType::ParenOpen && b == TokenType::ParenClose) ||
           (a == TokenType::BraceOpen && b == TokenType::BraceClose) ||
           (a == TokenType::BracketOpen && b == TokenType::BracketClose);
}
AstNode* node(const Token& t, AstNode* p) {
    auto* n = new AstNode{p, {}, t};
    if (p) p->children.push_back(n);
    return n;
}
Token label(const char* s) { return {TokenType::Unknown, s, 0, 0}; }

class Parser {
    const std::vector<Token>& ts;
    std::size_t p = 0;
    AstNode* root;
    TokenType peek() const { return p < ts.size() ? ts[p].type : TokenType::EndOfFile; }
    void skip() { while (p < ts.size() && trivia(ts[p].type)) ++p; }
    AstNode* group(AstNode* parent, TokenType end = TokenType::EndOfFile) {
        auto* n = node(label("expression"), parent);
        int depth = 0;
        while (p < ts.size()) {
            skip();
            if (peek() == end && depth == 0) break;
            if (peek() == TokenType::EndOfFile) break;
            Token t = ts[p++];
            if (open(t.type)) ++depth;
            if (close(t.type)) --depth;
            node(t, n);
            if (depth == 0 && (t.type == TokenType::Semicolon || t.type == TokenType::Colon ||
                                t.type == TokenType::BraceOpen || t.type == TokenType::BraceClose)) break;
        }
        return n;
    }
    void parens(AstNode* parent, bool forHeader = false) {
        if (peek() != TokenType::ParenOpen) return;
        auto* n = node(label(forHeader ? "for-header" : "condition"), parent); node(ts[p++], n);
        auto* part = node(label(forHeader ? "init" : "expression"), n);
        int depth = 0;
        while (p < ts.size() && peek() != TokenType::EndOfFile) {
            skip();
            if (peek() == TokenType::ParenClose && depth == 0) break;
            Token t = ts[p++];
            if (t.type == TokenType::ParenOpen) ++depth;
            if (t.type == TokenType::ParenClose) --depth;
            if (forHeader && t.type == TokenType::Semicolon && depth == 0) {
                node(t, part);
                const char* next = part->data.lexeme == "init" ? "condition" : "iteration";
                part = node(label(next), n);
            } else node(t, part);
        }
        if (peek() == TokenType::ParenClose) node(ts[p++], n);
    }
    void block(AstNode* parent) {
        if (peek() != TokenType::BraceOpen) return;
        auto* b = node(label("compound-statement"), parent); node(ts[p++], b);
        while (p < ts.size()) {
            skip();
            if (peek() == TokenType::BraceClose) { node(ts[p++], b); return; }
            statement(b);
        }
    }
    void statement(AstNode* parent) {
        skip();
        if (peek() == TokenType::BraceOpen) { block(parent); return; }
        TokenType t = peek();
        const std::string word = p < ts.size() ? ts[p].lexeme : std::string();
        if (word == "for") t = TokenType::For;
        else if (word == "while") t = TokenType::While;
        else if (word == "do") t = TokenType::Do;
        else if (word == "switch") t = TokenType::Switch;
        else if (word == "case") t = TokenType::Case;
        else if (word == "default") t = TokenType::Default;
        else if (word == "break") t = TokenType::Break;
        const char* name = nullptr;
        if (t == TokenType::Return) name = "return-statement";
        else if (t == TokenType::Break) name = "break-statement";
        else if (t == TokenType::Continue) name = "continue-statement";
        else if (t == TokenType::If) name = "if-statement";
        else if (t == TokenType::While) name = "while-statement";
        else if (t == TokenType::For) name = "for-statement";
        else if (t == TokenType::Do) name = "do-while-statement";
        else if (t == TokenType::Switch) name = "switch-statement";
        else if (t == TokenType::Case || t == TokenType::Default) name = "case-label";
        if (!name) { group(parent); return; }
        auto* s = node(label(name), parent); node(ts[p++], s);
        if (t == TokenType::If || t == TokenType::While || t == TokenType::For || t == TokenType::Switch) {
            parens(s, t == TokenType::For); if (peek() == TokenType::BraceOpen) block(s); else statement(s);
            if (t == TokenType::If && peek() == TokenType::Else) { node(ts[p++], s); statement(s); }
        } else if (t == TokenType::Do) {
            if (peek() == TokenType::BraceOpen) block(s); else statement(s);
            if (p < ts.size() && ts[p].lexeme == "while") {
                node(ts[p++], s); parens(s); if (peek() == TokenType::Semicolon) node(ts[p++], s);
            }
        } else if (t == TokenType::Case) { group(s, TokenType::Colon); if (peek() == TokenType::Colon) node(ts[p++], s); }
        else if (t == TokenType::Default) { if (peek() == TokenType::Colon) node(ts[p++], s); }
        else if (t == TokenType::Break || t == TokenType::Continue) {
            if (peek() == TokenType::Semicolon) node(ts[p++], s);
        } else { group(s); }
    }
public:
    Parser(const std::vector<Token>& v, AstNode* r) : ts(v), root(r) {}
    void parse() {
        while (p < ts.size()) {
            skip(); if (peek() == TokenType::EndOfFile) break;
            if (peek() == TokenType::Include || peek() == TokenType::Define || peek() == TokenType::Undef) {
                auto* d = node(label("preprocessor-directive"), root);
                const std::size_t line = ts[p].line;
                node(ts[p++], d);
                auto* path = node(label("include-path"), d);
                while (p < ts.size() && ts[p].type != TokenType::EndOfFile) {
                    if (ts[p].line != line) break;
                    if (!trivia(ts[p].type)) node(ts[p], path);
                    ++p;
                }
                continue;
            }
            std::size_t start = p, depth = 0, paren = 0;
            while (p < ts.size() && peek() != TokenType::EndOfFile) {
                if (peek() == TokenType::ParenOpen) ++paren;
                if (peek() == TokenType::ParenClose) --paren;
                if (peek() == TokenType::BraceOpen && paren == 0) { depth = 1; ++p; break; }
                if (peek() == TokenType::Semicolon && paren == 0) { ++p; break; }
                ++p;
            }
            if (depth) {
                auto* f = node(label("function-definition"), root);
                for (std::size_t i = start; i < p - 1; ++i) if (!trivia(ts[i].type)) node(ts[i], f);
                --p; block(f);
            } else {
                auto* d = node(label("external-declaration"), root);
                for (std::size_t i = start; i < p; ++i) if (!trivia(ts[i].type)) node(ts[i], d);
            }
        }
    }
};
}

void AstBuilder::destroy(AstNode* n) noexcept { if (!n) return; for (auto* c : n->children) destroy(c); delete n; }
AstNode* AstBuilder::clone(const AstNode* n, AstNode* p) {
    if (!n) return nullptr; auto* c = new AstNode{p, {}, n->data};
    try { for (auto* x : n->children) c->children.push_back(clone(x, c)); } catch (...) { destroy(c); throw; } return c;
}
AstBuilder::AstBuilder(const AstBuilder& o) : pos(o.pos), state(o.state), root(clone(o.root, nullptr)), tokens(o.tokens) {}
AstBuilder& AstBuilder::operator=(const AstBuilder& o) { if (this != &o) { auto* c=clone(o.root,nullptr); destroy(root); root=c; pos=o.pos; state=o.state; tokens=o.tokens; } return *this; }
AstBuilder::AstBuilder(AstBuilder&& o) noexcept : pos(o.pos), state(o.state), root(o.root), tokens(std::move(o.tokens)) { o.root=nullptr; o.pos=0; }
AstBuilder& AstBuilder::operator=(AstBuilder&& o) noexcept { if(this!=&o){destroy(root);root=o.root;pos=o.pos;state=o.state;tokens=std::move(o.tokens);o.root=nullptr;o.pos=0;}return *this; }
AstBuilder::~AstBuilder(){destroy(root);}


void AstBuilder::display() const {
    std::function<void(const AstNode*, std::size_t)> print = [&](const AstNode* n, std::size_t d) {
        std::cout << std::string(d * 2, ' ') << '(' << n->data.lexeme;
        if (n->children.empty()) { std::cout << ")\n"; return; }
        std::cout << '\n'; for (auto* c : n->children) print(c, d + 1); std::cout << std::string(d * 2, ' ') << ")\n";
    };
    if (root) print(root, 0);
}





namespace {
std::string flat(const AstNode* n) {
    if (!n) return {};
    if (n->children.empty()) return n->data.lexeme;
    std::string result;
    for (const auto* child : n->children) {
        const std::string value = flat(child);
        if (value.empty()) continue;
        if (!result.empty() && value != ";" && value != "," && value != ")" && value != "]") result += ' ';
        result += value;
    }
    return result;
}
const AstNode* childNamed(const AstNode* n, const std::string& name) {
    if (!n) return nullptr;
    for (const auto* c : n->children) if (c->data.lexeme == name) return c;
    return nullptr;
}
void formal(const AstNode* n, int indent) {
    if (!n) return;
    const std::string name = n->data.lexeme;
    const std::string pad(static_cast<std::size_t>(indent) * 4, ' ');
    if (name == "translation-unit") {
        std::cout << "根节点：\n";
        for (const auto* c : n->children) formal(c, indent + 1);
    } else if (name == "function-definition") {
        std::string ret, fn;
        for (const auto* c : n->children) {
            if (c->data.type == TokenType::Identifier && fn.empty()) fn = c->data.lexeme;
            else if (ret.empty() && c->data.type != TokenType::Identifier && c->data.type != TokenType::ParenOpen) ret = c->data.lexeme;
        }
        std::cout << pad << "函数名称:" << fn << " 函数返回类型:" << ret << "\n";
        for (const auto* c : n->children) if (c->data.lexeme == "compound-statement") formal(c, indent + 1);
    } else if (name == "compound-statement") {
        std::cout << pad << "语句块：\n";
        for (const auto* c : n->children) if (c->data.lexeme != "{" && c->data.lexeme != "}") formal(c, indent + 1);
    } else if (name == "for-statement") {
        std::cout << pad << "for循环：\n";
        for (const auto* c : n->children) if (c->data.lexeme != "for") formal(c, indent + 1);
    } else if (name == "for-header") {
        for (const auto* c : n->children) if (c->data.lexeme != "(" && c->data.lexeme != ")")
            std::cout << pad << c->data.lexeme << ": " << flat(c) << "\n";
    } else if (name == "while-statement") {
        std::cout << pad << "while循环：" << flat(childNamed(n, "condition")) << "\n";
        for (const auto* c : n->children) if (c->data.lexeme == "compound-statement") formal(c, indent + 1);
    } else if (name == "switch-statement") {
        std::cout << pad << "switch语句：\n";
        for (const auto* c : n->children) if (c->data.lexeme == "compound-statement") formal(c, indent + 1);
    } else if (name == "case-label") {
        std::cout << pad << "情况: " << flat(n) << "\n";
    } else if (name == "return-statement") {
        std::cout << pad << "返回: " << flat(n) << "\n";
    } else if (name == "expression") {
        std::cout << pad << "表达式: " << flat(n) << "\n";
    } else if (name == "break-statement" || name == "continue-statement") {
        std::cout << pad << (name == "break-statement" ? "break" : "continue") << "\n";
    } else {
        for (const auto* c : n->children) formal(c, indent);
    }
}
}

void AstBuilder::displayFormal() const { formal(root, 0); }

AstNode* AstBuilder::x_prase(std::vector<Token> input) {
    destroy(root); root=nullptr; pos=0; state=ParserContext::Normal; tokens=std::move(input);
    root=node(label("translation-unit"), nullptr);
    try { Parser(tokens, root).parse(); } catch (...) { destroy(root); root=nullptr; throw; }
    return root;
}
