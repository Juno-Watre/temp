以下是 CMake 项目命令行编译运行的常用方法：

  基本流程
  
  1. 生成构建系统

  # 在项目根目录下（CMakeLists.txt 所在目录）
  cmake -B build -DCMAKE_BUILD_TYPE=Debug

  - -B build：指定构建目录为 build
  - -DCMAKE_BUILD_TYPE=Debug：设置 Debug 模式（也可用 Release）

  2. 编译

  cmake --build build

  你也可以加 -j 指定并行编译的线程数：

  cmake --build build -j8

  3. 运行可执行文件

  # 可执行文件通常在 build 目录下
  ./build/你的可执行文件名

  如果你用的是 Visual Studio Generator（Windows），可执行文件路径通常类似：

  ./build/Debug/你的可执行文件名.exe

  常用变体

  Release 优化编译：
  cmake -B build -DCMAKE_BUILD_TYPE=Release
  cmake --build build

  生成 Ninja 构建（更快）：
  cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
  cmake --build build

  使用 CMakePresets.json（如果项目已配置）：
  cmake --preset default
  cmake --build --preset default

  一键配置+编译：
  cmake -B build -DCMAKE_BUILD_TYPE=Debug && cmake --build build -j8

  清理

  # 删除构建目录即可
  rm -rf build

  ./build/Debug/你的可执行文件名.exe

  常用变体

  Release 优化编译：
  cmake -B build -DCMAKE_BUILD_TYPE=Release
  cmake --build build

  生成 Ninja 构建（更快）：
  cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
  cmake --build build

  使用 CMakePresets.json（如果项目已配置）：
  cmake --preset default
  cmake --build --preset default

  一键配置+编译：
  cmake -B build -DCMAKE_BUILD_TYPE=Debug && cmake --build build -j8

  清理

  # 删除构建目录即可
  rm -rf build

  # 或者重新配置
  cmake -B build --fresh

  针对你的项目

  从 git status 来看，你的项目已经配置了 CMakePresets.json，可以直接用 preset：

  cmake --preset <preset名称>      # 配置
  cmake --build --preset <preset名称>  # 编译

  需要我帮你查看你现有的 CMake 配置，或者帮你编写 CMakeLists.txt 吗？

---

## 本项目解决方案（2026-09-01）

### 问题诊断

项目无法从命令行启动，共 3 个问题：

| # | 问题 | 影响 |
|---|------|------|
| 1 | `fmt` 库未安装，`find_package(fmt CONFIG REQUIRED)` 失败 | CMake 配置直接报错 |
| 2 | 源代码 bug：`lexer.cpp` 含 `#pragma once`，第38行 `\|\|` 运算符优先级导致越界 | 编译失败 |
| 3 | `CMakePresets.json` 仅配置 MSVC+cl.exe，但实际编译器是 MSYS2 GCC | 预设不可用 |

### 修改内容

#### 1. CMakeLists.txt — fmt 依赖
```cmake
# 改前：需要手动安装 vcpkg fmt
find_package(fmt CONFIG REQUIRED)

# 改后：FetchContent 自动下载（无需任何手动安装）
include(FetchContent)
FetchContent_Declare(fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG 11.1.4
)
FetchContent_MakeAvailable(fmt)
```

#### 2. src/lexer.cpp — Bug 修复
```cpp
// 删除第1行多余的 #pragma once（.cpp 文件不需要）

// 第38行：修复运算符优先级（|| → && 配合括号）
// 改前：while (pos < sources.length() && isalnum(sources[pos]) || sources[pos] == '_')
// 改后：while (pos < sources.length() && (isalnum(sources[pos]) || sources[pos] == '_'))
```

#### 3. CMakePresets.json — 新增 GCC 预设
```json
{
    "name": "gcc-debug",
    "generator": "Ninja",
    "binaryDir": "${sourceDir}/out/build/${presetName}",
    "cacheVariables": {
        "CMAKE_BUILD_TYPE": "Debug",
        "CMAKE_CXX_COMPILER": "g++",
        "CMAKE_EXPORT_COMPILE_COMMANDS": "ON"
    }
}
```

### 构建与运行

```bash
# 配置（首次自动下载 fmt 库）
cmake --preset gcc-debug

# 编译
cmake --build --preset gcc-debug

# 运行
./out/build/gcc-debug/Project1.exe
```

### clangd 配置（VS Code）

创建了两个文件：

**`.clangd`** — 指向编译数据库，开启 clang-tidy 和内联提示：
```yaml
CompileFlags:
  CompilationDatabase: out/build/gcc-debug
Diagnostics:
  ClangTidy:
    Add: [bugprone-*, modernize-*, performance-*, readability-*]
```

**`.vscode/settings.json`** — 禁用默认 C++ IntelliSense，避免冲突：
```json
{
    "clangd.path": "clangd",
    "C_Cpp.intelliSenseEngine": "disabled"
}
```

配置完成后执行 `clangd: Restart language server` 即可生效。

### 关键文件清单

| 文件 | 作用 |
|------|------|
| `CMakeLists.txt` | 构建配置，FetchContent 自动获取 fmt |
| `CMakePresets.json` | 预设：`gcc-debug` / `gcc-release`（MSYS2 GCC） |
| `.clangd` | clangd 语言服务器配置 |
| `.vscode/settings.json` | VS Code 集成配置 |
| `sources/*.txt` | 测试源码文件目录（运行时选择解析） |


clangd 的工作原理是：它不猜你的头文件在哪，全靠 compile_commands.json 告诉它。

  这个文件是 CMake 生成的编译数据库，里面记录了每个 .cpp 文件的确切编译命令：

  {
    "directory": ".../out/build/gcc-debug",
    "command": "g++ -IC:/.../include -IC:/.../fmt-src/include -std=c++20 -c src/main.cpp",
    "file": "src/main.cpp"
  }

  clangd 读到 -IC:/Users/m1590/Desktop/Project1/include 后，就知道 #include <compiler/ast.hpp> 应该去
  include/compiler/ast.hpp 找。

  所以修复链路很直：

  CMake 生成 compile_commands.json（含 -I include 路径）
          ↓
  .clangd 的 CompilationDatabase 指向那个目录
          ↓
  clangd 读取 → 知道所有头文件位置 → 补全/跳转/诊断全通

  之前不工作的原因就一个：没有 compile_commands.json。CMake 的 Ninja generator 默认不生成它，必须显式开启
  CMAKE_EXPORT_COMPILE_COMMANDS: ON。