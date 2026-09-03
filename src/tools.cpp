#include <compiler/tools.hpp>
#include <queue>
#include <string>
#include <iostream>

// waiting for test
void ToolsClass::display(AstNode *root) {
    std::queue<AstNode*> nodes;
    int nests = 0;
    while (true) {
        if (root->children.size() == 0) {
            nests --;
            break;
        }
        for (AstNode* node:root ->children) {
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


#include <string_view>
// 确保包含了你的 TokenType 枚举定义的头文件

std::string_view ToolsClass::tokenTypeToString(TokenType type) {
    switch (type) {
        // 1. 文件结束标记
        case TokenType::EndOfFile:        return "EndOfFile";

        // 2. 错误标记
        case TokenType::Error_type:       return "Error_type";

        // 3. 标识符
        case TokenType::Identifier:       return "Identifier";

        // 4. 字面量
        case TokenType::Integer:          return "Integer";
        case TokenType::Float:            return "Float";
        case TokenType::String:           return "String";
        case TokenType::Char:             return "Char";
        case TokenType::Boolean:          return "Boolean";
        case TokenType::Null:             return "Null";

        // 5.1 基本类型关键字
        case TokenType::Void_Key:            return "Void_";
        case TokenType::Int_Key:             return "Int_";
        case TokenType::Float_Key:           return "Float_";
        case TokenType::Double_Key:           return "Double";
        case TokenType::Char_Key:            return "Char_";
        case TokenType::Long:             return "Long";
        case TokenType::Short:            return "Short";
        case TokenType::Unsigned:         return "Unsigned";
        case TokenType::Signed:           return "Signed";
        case TokenType::Const:            return "Const";
        case TokenType::Static:           return "Static";
        case TokenType::Extern:           return "Extern";
        case TokenType::Volatile:         return "Volatile";
        case TokenType::Auto:             return "Auto";
        case TokenType::Register:         return "Register";

        // 5.2 控制流关键字
        case TokenType::If:               return "If";
        case TokenType::Else:             return "Else";
        case TokenType::While:            return "While";
        case TokenType::For:              return "For";
        case TokenType::Do:               return "Do";
        case TokenType::Switch:           return "Switch";
        case TokenType::Case:             return "Case";
        case TokenType::Break:            return "Break";
        case TokenType::Continue:         return "Continue";
        case TokenType::Return:           return "Return";
        case TokenType::Goto:             return "Goto";
        case TokenType::Default:          return "Default";

        // 5.3 函数与类型定义关键字
        case TokenType::Function:         return "Function";
        case TokenType::Struct:           return "Struct";
        case TokenType::Class:            return "Class";
        case TokenType::Union:            return "Union";
        case TokenType::Enum:             return "Enum";
        case TokenType::Typedef:          return "Typedef";
        case TokenType::Namespace:        return "Namespace";
        case TokenType::Template:         return "Template";
        case TokenType::Using:            return "Using";
        case TokenType::Operator:         return "Operator";

        // 5.4 内存与指针关键字
        case TokenType::New:              return "New";
        case TokenType::Delete:           return "Delete";
        case TokenType::Sizeof:           return "Sizeof";
        case TokenType::Alignof:          return "Alignof";

        // 5.5 异常处理关键字
        case TokenType::Try:              return "Try";
        case TokenType::Catch:            return "Catch";
        case TokenType::Throw:            return "Throw";
        case TokenType::Finally:          return "Finally";

        // 5.6 访问控制与修饰
        case TokenType::Public:           return "Public";
        case TokenType::Private:          return "Private";
        case TokenType::Protected:        return "Protected";
        case TokenType::Virtual:          return "Virtual";
        case TokenType::Inline:           return "Inline";
        case TokenType::Explicit:         return "Explicit";
        case TokenType::Mutable:          return "Mutable";

        // 5.7 其他关键字
        case TokenType::True:             return "True";
        case TokenType::False:            return "False";
        case TokenType::Nullptr:          return "Nullptr";
        case TokenType::StaticAssert:     return "StaticAssert";
        case TokenType::Noexcept:         return "Noexcept";
        case TokenType::AutoType:         return "AutoType";

        // 6.1 算术运算符
        case TokenType::Plus:             return "Plus";
        case TokenType::Minus:            return "Minus";
        case TokenType::Multiply:         return "Multiply";
        case TokenType::Divide:           return "Divide";
        case TokenType::Modulo:           return "Modulo";
        case TokenType::Increment:        return "Increment";
        case TokenType::Decrement:        return "Decrement";

        // 6.2 赋值运算符
        case TokenType::Assign:           return "Assign";
        case TokenType::AddAssign:        return "AddAssign";
        case TokenType::SubAssign:        return "SubAssign";
        case TokenType::MulAssign:        return "MulAssign";
        case TokenType::DivAssign:        return "DivAssign";
        case TokenType::ModAssign:        return "ModAssign";

        // 6.3 比较运算符
        case TokenType::Equal:            return "Equal";
        case TokenType::NotEqual:         return "NotEqual";
        case TokenType::Greater:          return "Greater";
        case TokenType::Less:             return "Less";
        case TokenType::GreaterEqual:     return "GreaterEqual";
        case TokenType::LessEqual:        return "LessEqual";

        // 6.4 逻辑运算符
        case TokenType::LogicalAnd:       return "LogicalAnd";
        case TokenType::LogicalOr:        return "LogicalOr";
        case TokenType::LogicalNot:       return "LogicalNot";

        // 6.5 位运算符
        case TokenType::BitwiseAnd:       return "BitwiseAnd";
        case TokenType::BitwiseOr:        return "BitwiseOr";
        case TokenType::BitwiseXor:       return "BitwiseXor";
        case TokenType::BitwiseNot:       return "BitwiseNot";
        case TokenType::ShiftLeft:        return "ShiftLeft";
        case TokenType::ShiftRight:       return "ShiftRight";

        // 6.6 其他运算符
        case TokenType::Arrow:            return "Arrow";
        case TokenType::Dot:              return "Dot";
        case TokenType::Comma:            return "Comma";
        case TokenType::QuestionMark:     return "QuestionMark";
        case TokenType::Colon:            return "Colon";
        case TokenType::Ellipsis:         return "Ellipsis";

        // 7. 括号与分隔符
        case TokenType::ParenOpen:        return "ParenOpen";
        case TokenType::ParenClose:       return "ParenClose";
        case TokenType::BraceOpen:        return "BraceOpen";
        case TokenType::BraceClose:       return "BraceClose";
        case TokenType::BracketOpen:      return "BracketOpen";
        case TokenType::BracketClose:     return "BracketClose";

        // 8. 标点符号
        case TokenType::Semicolon:        return "Semicolon";
        case TokenType::CommaSep:         return "CommaSep";
        case TokenType::DotSep:           return "DotSep";
        case TokenType::AtSign:           return "AtSign";

        // 9. 预处理指令
        case TokenType::Include:          return "Include";
        case TokenType::Define:           return "Define";
        case TokenType::Undef:            return "Undef";
        case TokenType::IfDef:            return "IfDef";
        case TokenType::IfNDef:           return "IfNDef";
        case TokenType::ElsePre:          return "ElsePre";
        case TokenType::EndIf:            return "EndIf";
        case TokenType::Pragma:           return "Pragma";
        case TokenType::Line:             return "Line";
        case TokenType::Error:            return "Error";

        // 10. 特殊标记
        case TokenType::Unknown:          return "Unknown";
        case TokenType::Whitespace:       return "Whitespace";
        case TokenType::Comment:          return "Comment";

        // 安全兜底：防止未定义行为
        default:                          return "UNKNOWN_TOKEN";
    }
}