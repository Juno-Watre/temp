# 本项目常用 CMake 命令

## 构建

```bash
# 配置（Debug，首次自动下载 fmt 库）
cmake --preset gcc-debug

# 配置（Release）
cmake --preset gcc-release

# 编译
cmake --build --preset gcc-debug

# 编译（Release）
cmake --build --preset gcc-release
```

## 一键配置+编译

```bash
cmake --preset gcc-debug && cmake --build --preset gcc-debug
```

## 运行

```bash
./out/build/gcc-debug/Project1.exe
```

## 清理

```bash
# 删除构建产物
rm -rf out/build/gcc-debug

# 或使用 CMake 重新配置（保留下载的依赖）
cmake --preset gcc-debug --fresh
```




构建
powershell

编辑



# 配置（Debug，首次自动下载 fmt 库）
cmake --preset gcc-debug

# 配置（Release）
cmake --preset gcc-release

# 编译
cmake --build --preset gcc-debug

# 编译（Release）
cmake --build --preset gcc-release
一键配置+编译
powershell

编辑



cmake --preset gcc-debug; cmake --build --preset gcc-debug
运行
powershell

编辑



.\out\build\gcc-debug\Project1.exe
清理
powershell

编辑



# 删除构建产物
Remove-Item -Recurse -Force out\build\gcc-debug

# 或使用 CMake 重新配置（保留下载的依赖）
cmake --preset gcc-debug --fresh