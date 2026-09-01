const std::string file_path = "C:\\Users\\m1590\\Desktop\\Project1\\sources\\";

// read file from given path :
// given path is const std::string file_path
#include <iostream>
#include <fstream>
#include <filesystem> 
#include <string>
#include <vector>

namespace fs = std::filesystem;

class FileRead {
public:
    // 1. 读取指定路径文件的完整内容
    static std::string get_file_content(std::string file_name) {
        file_name = file_path + file_name;
        std::ifstream in(file_name, std::ios::in | std::ios::binary);
        if (!in.is_open()) {
            std::cerr << "Error: Cannot open file " << file_name << std::endl;
            return "";
        }

        std::string content;
        std::string line;
        // 逐行读取并拼接到 content 中
        while (std::getline(in, line)) {
            content += line + "\n";
        }

        in.close();
        return content;
    }

    // 2. 获取指定目录下的所有文件路径列表
    static std::vector<std::string> get_file_list() {
        std::string dir_path = file_path;
        std::vector<std::string> file_list;

        // 使用 C++17 的 filesystem 遍历目录
        try {
            for (const auto& entry : fs::directory_iterator(dir_path)) {
                if (entry.is_regular_file()) {
                    file_list.push_back(entry.path().string());
                }
            }
        }
        catch (const fs::filesystem_error& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }

        return file_list;
    }
};