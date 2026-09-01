 建议从一开始就采用通用的 CMake 项目结构。Visual Studio、VS Code、CLion 和命令行都能直接使用，不会绑定某个 IDE。

  source-analyzer/
  ├─ CMakeLists.txt             # 项目构建规则
  ├─ vcpkg.json                 # 第三方依赖声明（可选）
  ├─ README.md                  # 项目说明、运行方式
  │
  ├─ include/                   # 头文件：声明“有什么功能”
  │  └─ compiler/
  │     ├─ token.hpp            # Token 类型、TokenKind 枚举
  │     ├─ lexer.hpp            # 词法分析器接口
  │     ├─ ast.hpp              # 抽象语法树节点定义
  │     ├─ parser.hpp           # 语法分析器接口
  │     └─ diagnostic.hpp       # 错误信息与位置
  │
  ├─ src/                       # 源文件：实现功能
  │  ├─ main.cpp                # 程序入口
  │  ├─ lexer.cpp               # 识别关键字、标识符、常量、运算符等
  │  ├─ ast.cpp                 # 创建、打印 AST
  │  ├─ parser.cpp              # 递归下降语法分析
  │  └─ diagnostic.cpp          # 输出词法或语法错误
  │
  ├─ tests/                     # 自动测试
  │  ├─ lexer_tests.cpp
  │  ├─ parser_tests.cpp
  │  └─ expression_tests.cpp
  │
  ├─ examples/                  # 手工测试的源程序
  │  ├─ valid.c
  │  └─ invalid.c
  │
  └─ docs/                      # 实验文档
     └─ grammar.md              # 实际支持的文法

  先只完成最必要的四个模块：

  main
   └─ Lexer：源代码文本 -> Token 序列
       └─ Parser：Token 序列 -> AST
           └─ AST Printer：AST -> 控制台树形输出

  对新手而言，可以先不创建 tests/、docs/ 和 vcpkg.json，初始最小版本只需要：

  include/compiler/token.hpp
  include/compiler/lexer.hpp
  src/main.cpp
  src/lexer.cpp
  CMakeLists.txt
  examples/valid.c

  等词法分析能正确输出 Token 后，再加入 ast 与 parser。第三方包也可以后加，推荐顺序是先完全不依赖，后续仅加入 doctest 测
  试和 fmt 格式化输出。