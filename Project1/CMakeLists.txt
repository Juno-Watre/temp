# CMakeList.txt: Project1 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#

# 将源代码添加到此项目的可执行文件。
add_executable(Project1
    src/main.cpp
)

target_compile_features(Project1 PRIVATE cxx_std_17)

# 让 #include <project1/example.hpp> 能找到 include/ 中的头文件。
target_include_directories(Project1 PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# TODO: 如有需要，请添加测试并安装目标。
