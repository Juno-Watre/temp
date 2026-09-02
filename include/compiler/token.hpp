#pragma once

#include <string>

enum class TokenType
{
    // ========================
    // 1. 文件结束标记
    // ========================
    EndOfFile,        // EOF: 源文件结束，词法分析器到达文件末尾时返回

    // ========================
    // 2. 错误标记
    // ========================
    Error_type,            // 无法识别的字符序列，词法分析器遇到非法输入时返回

    // ========================
    // 3. 标识符（用户自定义名称）
    // ========================
    Identifier,       // 变量名、函数名、类名、结构体名等用户自定义标识符

    // ========================
    // 4. 字面量（Literal）
    // ========================
    Integer,          // 整型字面量：十进制(123)、十六进制(0xFF)、八进制(0o77)、二进制(0b1010)
    Float,            // 浮点数字面量：3.14、1.5e-10、2.0F、3.14L
    String,          // 字符串字面量："hello world"、"C++\0"
    Char,            // 字符字面量：'a'、'\\n'、'\\t'、'\\''、'\\0'
    Boolean,         // 布尔字面量：true、false
    Null,            // 空指针字面量：nullptr、NULL

    // ========================
    // 5. 关键字（Keyword）—— 语言保留字
    // ========================
    // --- 5.1 基本类型关键字 ---
    Void_,             // void: 空类型，用于函数返回类型或空指针声明
    Int_,              // int: 整型
    Float_,            // float: 单精度浮点型（注意：与字面量Float区分，此为类型关键字）
    Double,           // double: 双精度浮点型
    Char_,             // char: 字符型（类型关键字，与字面量Char区分）
    Bool,             // bool: 布尔型
    Long,             // long: 长整型
    Short,            // short: 短整型
    Unsigned,         // unsigned: 无符号修饰符
    Signed,           // signed: 有符号修饰符
    Const,            // const: 常量修饰符
    Static,           // static: 静态存储修饰符
    Extern,           // extern: 外部链接修饰符
    Volatile,         // volatile: 易变修饰符
    Auto,             // auto: 自动类型推导（C++11）
    Register,         // register: 寄存器存储提示（已废弃）

    // --- 5.2 控制流关键字 ---
    If,               // if: 条件分支语句
    Else,             // else: if的否定分支
    While,            // while: 当型循环
    For,              // for: 计数循环
    Do,               // do: 直到型循环（do-while）
    Switch,           // switch: 多分支选择
    Case,             // case: switch的分支标签
    Break,            // break: 跳出循环或switch
    Continue,         // continue: 跳过本次循环迭代
    Return,           // return: 函数返回值
    Goto,             // goto: 无条件跳转（不推荐但语言支持）
    Default,          // default: switch的默认分支

    // --- 5.3 函数与类型定义关键字 ---
    Function,         // function: 函数定义（课程自定义）
    Struct,           // struct: 结构体定义
    Class,            // class: 类定义
    Union,            // union: 联合体定义
    Enum,             // enum: 枚举定义
    Typedef,          // typedef: 类型别名定义
    Namespace,        // namespace: 命名空间（C++）
    Template,         // template: 模板定义（C++）
    Using,            // using: 命名空间/类型引用（C++）
    Operator,         // operator: 运算符重载（C++）

    // --- 5.4 内存与指针关键字 ---
    New,              // new: 动态内存分配
    Delete,           // delete: 释放动态内存
    Sizeof,           // sizeof: 获取类型/变量大小
    Alignof,          // alignof: 获取对齐要求（C++11）

    // --- 5.5 异常处理关键字 ---
    Try,              // try: 异常捕获块开始
    Catch,            // catch: 异常捕获
    Throw,            // throw: 抛出异常
    Finally,          // finally: 无论是否异常都执行的块

    // --- 5.6 访问控制与修饰 ---
    Public,           // public: 公有访问权限
    Private,          // private: 私有访问权限
    Protected,        // protected: 受保护访问权限
    Virtual,          // virtual: 虚函数/虚基类
    Inline,           // inline: 内联函数提示
    Explicit,         // explicit: 禁止隐式转换（C++）
    Mutable,          // mutable: 可修改的const成员（C++）

    // --- 5.7 其他关键字 ---
    True,             // true: 布尔真值
    False,            // false: 布尔假值
    Nullptr,          // nullptr: 空指针常量（C++11）
    StaticAssert,     // static_assert: 编译期断言（C++11）
    Noexcept,         // noexcept: 不抛出异常声明（C++11）
    AutoType,         // auto: 自动类型推导（C++11，与auto关键字同名）

    // ========================
    // 6. 运算符（Operator）
    // ========================
    // --- 6.1 算术运算符 ---
    Plus,             // +: 加法运算符或正号
    Minus,            // -: 减法运算符或负号
    Multiply,         // *: 乘法运算符
    Divide,           // /: 除法运算符
    Modulo,           // %: 取模（取余）运算符
    Increment,        // ++: 自增运算符（前缀/后缀）
    Decrement,        // --: 自减运算符（前缀/后缀）

    // --- 6.2 赋值运算符 ---
    Assign,           // =: 赋值运算符
    AddAssign,        // +=: 加后赋值
    SubAssign,        // -=: 减后赋值
    MulAssign,        // *=: 乘后赋值
    DivAssign,        // /=: 除后赋值
    ModAssign,        // %=: 取模后赋值

    // --- 6.3 比较运算符 ---
    Equal,            // ==: 等于比较
    NotEqual,         // !=: 不等于比较
    Greater,          // >: 大于比较
    Less,             // <: 小于比较
    GreaterEqual,     // >=: 大于等于比较
    LessEqual,        // <=: 小于等于比较

    // --- 6.4 逻辑运算符 ---
    LogicalAnd,       // &&: 逻辑与（短路求值）
    LogicalOr,        // ||: 逻辑或（短路求值）
    LogicalNot,       // !: 逻辑非

    // --- 6.5 位运算符 ---
    BitwiseAnd,       // &: 按位与
    BitwiseOr,        // |: 按位或
    BitwiseXor,       // ^: 按位异或
    BitwiseNot,       // ~: 按位取反
    ShiftLeft,        // <<: 左移运算符（也用于流插入）
    ShiftRight,       // >>: 右移运算符（也用于流提取）

    // --- 6.6 其他运算符 ---
    Arrow,            // ->: 成员访问运算符（指针访问）
    Dot,              // .: 成员访问运算符（对象访问）
    Comma,            // ,: 逗号运算符（顺序求值）
    QuestionMark,     // ?: 三元条件运算符
    Colon,            // :: 范围解析运算符 / 类型限定符
    Ellipsis,         // ...: 可变参数模板 / 省略号

    // ========================
    // 7. 括号与分隔符（Bracket / Delimiter）
    // ========================
    ParenOpen,        // (: 左圆括号，用于函数调用、表达式分组
    ParenClose,       // ): 右圆括号
    BraceOpen,        // {: 左花括号，用于函数体、代码块、结构体定义
    BraceClose,       // }: 右花括号
    BracketOpen,      // [: 左方括号，用于数组下标访问
    BracketClose,     // ]: 右方括号

    // ========================
    // 8. 标点符号（Punctuation）
    // ========================
    Semicolon,        // ;: 语句结束分隔符
    CommaSep,         // ,: 参数/声明分隔符（与逗号运算符Comma区分）
    DotSep,           // .: 成员访问点号（与运算符Dot区分，此处作为分隔符）
    AtSign,           // @: 特殊标记（如装饰器、属性等）

    // ========================
    // 9. 预处理指令（Preprocessor）
    // ========================
    Include,          // #include: 文件包含指令
    Define,           // #define: 宏定义指令
    Undef,            // #undef: 取消宏定义
    IfDef,            // #ifdef: 条件编译（如果已定义）
    IfNDef,           // #ifndef: 条件编译（如果未定义）
    ElsePre,          // #else: 条件编译否定分支
    EndIf,            // #endif: 条件编译结束
    Pragma,           // #pragma: 编译指令
    Line,             // #line: 行号控制
    Error,            // #error: 编译期报错

    // ========================
    // 10. 特殊标记
    // ========================
    Unknown,          // 未知token，用于占位或调试
    Whitespace,       // 空白字符（空格、制表符、换行），通常被跳过
    Comment,          // 注释标记（// 单行注释 / /* ... */ 多行注释）
};



struct Token
{
    TokenType type;       // token type
    std::string lexeme;   // raw text
    size_t line;          // line number
    size_t column;        // column number

    Token& operator= (const Token& other) = default;
};