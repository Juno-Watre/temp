#pragma once

#include <vector>
#include <cstddef>
#include <compiler/token.hpp>

struct AstNode
{
    AstNode* par;
    std::vector<AstNode*> children;
    Token data;

    AstNode& operator=(const AstNode& other);
};

enum class ParserContext {
    // ==================== 预处理与词法修正 ====================
    Normal,             // 默认上下文：标准 C 语句/表达式解析

    PreprocessorLine,   // # 之后到换行符之间
                        // 作用: 禁用普通语句解析，识别 define/ifdef/include 等指令关键字
                        // 注意: 此状态下 \n 是终止符而非空白符
    
    IncludePath,        // #include < 或 #include " 之后
                        // 作用: 将 < > 或 " " 之间的所有 token 视为路径字符序列
                        // 解决: < 被误解析为 LT，> 被误解析为 GT 的问题
    
    MacroBody,          // #define MACRO 之后的替换体
                        // 作用: 允许 ## (token粘贴) 和 # (字符串化) 运算符
                        // 解决: ## 在 Normal 上下文中是非法 token
    
    // ==================== 语法歧义消解 ====================
    DeclOrExpr,         // 语句开头遇到 IDENTIFIER 时
                        // 作用: 向前窥探(peahead)判断是类型名(声明)还是变量名(表达式)
                        // 解决: C 语言经典的 typedef/变量名 二义性
    
    StructOrUnionBody,  // struct/union { ... } 内部
                        // 作用: 只允许声明，禁止表达式语句；处理位域 ':' 
                        // 解决: 位域语法 x : 4 中的 ':' 与普通标签/三元运算符冲突
    
    ParamList,          // 函数声明/定义的参数列表 (...) 内
                        // 作用: 允许抽象声明符(省略参数名)，识别 void 特殊含义
                        // 解决: int func(int) 合法，但 int x = int 非法
};

class AstBuilder {
private:
    std::size_t pos = 0;
    ParserContext state = ParserContext::Normal;
    AstNode* root = nullptr;
    std::vector<Token> tokens;

    static void destroy(AstNode* node) noexcept;
    static AstNode* clone(const AstNode* node, AstNode* parent);

public:
    AstBuilder() = default;
    AstBuilder(const AstBuilder& other);
    AstBuilder& operator=(const AstBuilder& other);
    AstBuilder(AstBuilder&& other) noexcept;
    AstBuilder& operator=(AstBuilder&& other) noexcept;
    ~AstBuilder();

    AstNode* x_prase(std::vector<Token> input);
    void display() const;
    // 面向用户的语义化 AST 展示，display() 保留为调试输出。
    void displayFormal() const;
    [[nodiscard]] AstNode* tree() const noexcept { return root; }
    static void destroyTree(AstNode* node) noexcept { destroy(node); }
};