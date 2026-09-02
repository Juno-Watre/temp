#pragma once

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>

const std::string file_path = "/home/SuFin/desktop/D/Project1/sources/";

namespace fs = std::filesystem;

class FileRead {
public:
    // 1. read file content from given path
    static std::string get_file_content(const std::string& file_name) {
        std::ifstream in_stream(file_name, std::ios::in | std::ios::binary);
        if (!in_stream.is_open()) {
            std::cerr << "Error: Cannot open file " << file_name << '\n';
            return "";
        }

        std::string content;
        std::string line;
        // read line by line, append to content
        while (std::getline(in_stream, line)) {
            content += line + "\n";
        }

        in_stream.close();
        return content;
    }

    // 2. list all file paths in the target directory
    static std::vector<std::string> get_file_list() {
        std::string dir_path = file_path;
        std::vector<std::string> file_list;

        // use C++17 filesystem to iterate directory
        try {
            for (const auto& entry : fs::directory_iterator(dir_path)) {
                if (entry.is_regular_file()) {
                    file_list.push_back(entry.path().string());
                }
            }
        }
        catch (const fs::filesystem_error& e) {
            std::cerr << "Error: " << e.what() << '\n';
        }

        return file_list;
    }
};