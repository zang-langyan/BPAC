# BPAC++

[中文](#文献管理系统-bpac) | [English](#bibliograph-preview-and-citation-system-bpac)

## 文献管理系统 (BPAC)

**BPAC++** 是一个基于C++编写的文献预览与管理软件。它可以使你在管理文献的同时进行预览，并且在预览的过程中给文献添加笔记、修改属性、复制引用代码。你还可以将整理好的文献仓库导出为`.bib`格式和`.md`格式。导出的`.bib`文件中包含了所有整理好的文献；而`.md`文件中则将文献内容和你的笔记整理成表格。

### 安装和贡献

目前的版本尚未有预编译的安装包，敬请谅解。

在编译之前需要下载必要的库到你的系统中 --  `glfw`, `glew`, `poppler`。

* Mac OS
```sh
brew install glfw glew poppler
```

Windows 和 Linux 请见`Makefile`。（尚未测试）

* 编译和运行
```sh
make
./BPAC++
```

### 目前的开发状态

目前完全由[臧朗彦](https://github.com/7lang2yan?tab=repositories)独立开发。如果你愿意参与到开发中来欢迎发送 issues 和 pull requests。


## Bibliograph Preview And Citation system (BPAC)

**BPAC++** is a C++ based bibliography previewer and manager built on top of OpenGL, Dear ImGui and Poppler. The name stands for ***"Bibliography Preview And Citation"***. It can preview and manage literatures from different locations of your computer and can export .bib file based on your management. It can also export markdown file with a table of literatures and notes you provide. 

### Installation/Contribution

Before compiling and running, you have to download the necessary libraries to your OS, namely `glfw`, `glew`, `poppler`. 

* Mac OS
```sh
brew install glfw glew poppler
```

Windows and Linux see `Makefile` for details.

* Compile and Run
```sh
make
./BPAC++
```

### Current State

This repository is currently solely by [Langyan Zang](https://github.com/7lang2yan?tab=repositories). Pull requests and issues are welcome.

