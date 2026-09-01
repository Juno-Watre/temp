cmake_minimum_required(VERSION 3.20)

project(Project1 LANGUAGES CXX)

add_executable(Project1
    src/main.cpp
 "src/lexer.cpp" "src/ast.cpp" )

target_compile_features(Project1 PRIVATE cxx_std_17)

target_include_directories(Project1 PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

find_package(fmt CONFIG REQUIRED)
target_link_libraries(Project1 PRIVATE fmt::fmt)
