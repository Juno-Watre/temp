#include <string>
#include <iostream>

// 解决 Windows 控制台中文 UTF-8 乱码问题
#ifdef _WIN32
#include <windows.h>
#pragma execution_character_set("utf-8")
#endif

#include <compiler/token.hpp>
#include <compiler/fileread.hpp>
#include <compiler/ast.hpp>
#include <compiler/lexer.hpp>
#include <compiler/tools.hpp>



int main() {
#ifdef _WIN32
    // 设置控制台输出代码页为 UTF-8
    SetConsoleOutputCP(CP_UTF8);
#endif

    bool running = true;
    do {
        std::cout << "\n========== 数字字符解析系统 ==========\n";
        
        // 1. 获取并展示文件列表
        std::vector<std::string> fileList = FileRead::get_file_list();
        if (fileList.empty()) {
            std::cout << "[提示] 当前目录下没有找到可解析的文件。\n";
            break;
        }

        std::cout << "请选择要解析的文件：\n";
        for (size_t i = 0; i < fileList.size(); ++i) {
            std::cout << "  [" << i << "] " << fileList[i] << "\n";
        }
        std::cout << "======================================\n> ";

        // 2. 安全获取用户选择的数字（带输入校验）
        int input_select_num;
        while (!(std::cin >> input_select_num) || 
               input_select_num < 0 || 
               input_select_num >= static_cast<int>(fileList.size())) {
            
            std::cout << "[错误] 输入无效！请输入 0 到 " << fileList.size() - 1 << " 之间的数字：> ";
            std::cin.clear(); // 清除错误状态
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // 清空输入缓冲区
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // 清理正常输入后的换行符

        // 3. 执行解析逻辑
        std::string file_content = FileRead::get_file_content(fileList[input_select_num]);
        std::cout << "\n--- 文件内容 ---\n" << file_content << "\n----------------\n";

        Lexer lexer(file_content);
        lexer.letex_analyzer();
        
        AstGenerator ast_tree(lexer.tokens);
        Node* root = ast_tree.geneAstTree();
        ToolsClass::display(root);

        // 4. 询问是否继续
        std::cout << "\n是否继续解析其他文件？(1: 退出 / 0: 继续): > ";
        int continue_flag;
        while (!(std::cin >> continue_flag)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "[错误] 请输入数字 (1 或 0): > ";
        }
        
        if (continue_flag == 1) {
            running = false;
        }

    } while (running);

    std::cout << "程序已退出。\n";
    return 0;
}