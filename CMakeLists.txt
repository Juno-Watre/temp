cmake_minimum_required(VERSION 3.20)

project(Project1 LANGUAGES CXX)

add_executable(Project1
    src/main.cpp
    src/lexer.cpp
    src/ast.cpp
    src/tools.cpp
)

target_compile_features(Project1 PRIVATE cxx_std_20)

target_include_directories(Project1 PRIVATE include)

# clangd 配置
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)