cmake_minimum_required(VERSION 3.20)

project(Project1 LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG        11.1.4
)
FetchContent_MakeAvailable(fmt)

add_executable(Project1
    src/main.cpp
    src/lexer.cpp
    src/ast.cpp
)

target_compile_features(Project1 PRIVATE cxx_std_20)

target_include_directories(Project1 PRIVATE include)

target_link_libraries(Project1 PRIVATE fmt::fmt)