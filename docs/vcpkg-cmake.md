# 在 CMake 项目中使用 vcpkg

本项目推荐使用 vcpkg 的 **manifest 模式**：项目根目录的 `vcpkg.json`
记录依赖，其他人获取项目后也能安装相同的包。

下面以 `fmt` 为例。`fmt` 用于更方便地格式化控制台输出；它只是示例，
当前项目不需要为了完成实验而安装它。

## 1. 使用 Visual Studio 自带的 vcpkg

如果在 Visual Studio Installer 中安装了 C++ 工作负载及 `vcpkg package manager`
组件，vcpkg 已随 Visual Studio 安装，无需再次下载或克隆。

它通常位于以下目录（将 `<Edition>` 替换为 Community、Professional 或 Enterprise）：

```text
C:\Program Files\Microsoft Visual Studio\2022\<Edition>\VC\vcpkg

C:\Program Files\Microsoft Visual Studio\2022\Community\VC\vcpkg
```

将实际目录设置为系统环境变量 `VCPKG_ROOT`。重新打开 Visual Studio 或终端后，可用
下面的命令检查：

```powershell
echo $env:VCPKG_ROOT
```

输出应是 vcpkg 的安装目录。如果目录不同，以实际包含 `vcpkg.exe` 和 `scripts`
子目录的位置为准。

> `vcpkg integrate install` 主要帮助传统的 Visual Studio `.sln` 项目自动找到包。
> 本项目是 CMake 项目，关键配置是后面的 `CMAKE_TOOLCHAIN_FILE`，不要只依赖
> `vcpkg integrate install`。

## 2. 在项目根目录声明依赖

在与顶层 `CMakeLists.txt` 同级的位置新建 `vcpkg.json`：

```json
{
  "name": "project1",
  "version-string": "0.1.0",
  "dependencies": [
    "fmt"
  ]
}
```

以后需要新包时，只把包名加入 `dependencies`，例如：

```json
"dependencies": ["fmt", "doctest"]
```

第一次 CMake 配置时，vcpkg 会自动下载并构建 `vcpkg.json` 中的包。不要把
`vcpkg_installed/` 目录提交到 Git，它是生成文件。

## 3. 让 CMake 使用 vcpkg

编辑根目录的 `CMakePresets.json`。在 `windows-base` 预设的
`cacheVariables` 中加入下列项：

```json
"CMAKE_TOOLCHAIN_FILE": "$env{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
```

加入后的关键部分如下：

```json
"cacheVariables": {
  "CMAKE_C_COMPILER": "cl.exe",
  "CMAKE_CXX_COMPILER": "cl.exe",
  "CMAKE_TOOLCHAIN_FILE": "$env{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
}
```

这一步必须在第一次配置 CMake 时生效。如果之前已用同一个 preset 配置过项目，
Visual Studio 中选择“删除缓存并重新配置”，或删除对应的 `out/build/<preset>`
构建目录后重新配置。

## 4. 在子项目中查找并链接包

本项目的可执行文件 `Project1` 定义在 `Project1/CMakeLists.txt`。在
`target_include_directories(...)` 之后添加：

```cmake
find_package(fmt CONFIG REQUIRED)
target_link_libraries(Project1 PRIVATE fmt::fmt)
```

`find_package` 查找 vcpkg 安装的包，`target_link_libraries` 将它链接到指定目标。
不要手动填写 `.lib` 文件路径，也不要手动添加 vcpkg 的 `include` 目录。

## 5. 在 C++ 中使用包

在 `Project1/src/main.cpp` 中：

```cpp
#include <fmt/format.h>
#include <project1/example.hpp>

int main() {
    fmt::print("{}\\n", project1::welcome_message());
}
```

重新配置并生成项目后运行，程序将通过 fmt 输出文字。

## 常见问题

- CMake 报 `Could not find a package configuration file provided by "fmt"`：
  检查 `vcpkg.json` 是否在项目根目录，并确认 `CMAKE_TOOLCHAIN_FILE` 已配置后重新配置。
- 提示找不到 `cl.exe`：在 Visual Studio Installer 安装“使用 C++ 的桌面开发”；
  命令行构建则使用“Developer PowerShell for VS”。
- 安装的包没有生效：确认修改的是当前使用的 preset，并执行“删除缓存并重新配置”。
- 不同包的 CMake 目标名不一定等于包名：在 vcpkg 的包说明或该库文档中查找其
  `find_package(...)` 和 `target_link_libraries(...)` 示例。
