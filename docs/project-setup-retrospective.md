# 本次工程配置复盘

## 现象

项目一开始存在两个 `CMakeLists.txt`：根目录一个，`Project1/` 目录中还有一个。
源文件也位于 `Project1/src`，形成了 `Project1/Project1` 的嵌套目录。

## 原因

这是 Visual Studio 的 CMake 新建项目模板默认创建的“顶层工程 + 子项目”结构。
这种结构适合一个仓库包含多个可执行程序或多个库的情况，例如：

```text
根目录
├─ app1/CMakeLists.txt
├─ app2/CMakeLists.txt
└─ library/CMakeLists.txt
```

当前项目只有一个可执行程序，因此保留这层结构只会增加查找文件和理解配置的成本。

## 这次需要避免的问题

1. **小项目过早使用多层 CMake。** 对单个程序，目标、依赖和源文件都在根目录的
   一个 `CMakeLists.txt` 中更直观。
2. **目标与其依赖写在不同文件。** 原先 `Project1` 目标在内层创建，但 `fmt` 的
   `find_package` 和 `target_link_libraries` 在外层。CMake 可以处理这种写法，但阅读者
   不容易判断一个目标依赖了什么。现在它们已放在一起。
3. **将本机绝对路径写入预设。**
   `C:\\Program Files\\...\\vcpkg` 只适用于当前电脑和当前 Visual Studio 版本。
   现在改为 `$env{VCPKG_ROOT}`，每台电脑只需配置自己的 `VCPKG_ROOT`。
4. **忽略 JSON 格式细节。** `CMakePresets.json` 的 `x86-debug` 预设曾在最后一个
   属性后保留逗号，严格 JSON 不允许这种写法；现已移除。

## 当前简化后的结构

```text
Project1/
├─ CMakeLists.txt
├─ CMakePresets.json
├─ vcpkg.json
├─ include/project1/example.hpp
├─ src/main.cpp
└─ docs/
```

当项目未来出现独立库、测试程序或多个可执行程序时，再使用
`add_subdirectory()` 拆分为多个 CMake 文件。
