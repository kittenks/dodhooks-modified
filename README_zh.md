# DODHooks 中文构建指南

## 项目简介

DODHooks 是一个为 **Day of Defeat: Source**（胜利之日：起源）游戏服务器提供的 SourceMod 扩展，它通过函数钩子（Detours）和原生函数（Natives）让插件开发者能够拦截和修改游戏内部行为。

### 主要功能

| 类别 | 内容 |
|------|------|
| 钩子（Detours） | 语音命令、加入兵种、头盔弹飞、重生、加时、设置获胜队伍、回合状态、玩家状态、炸弹目标状态 |
| 原生函数（Natives） | 获取/设置玩家兵种、控制点图标管理、计时器控制、强制重生、设置回合状态 |
| 转发（Forwards） | 上述所有事件的插件级回调钩子 |

### 与原版的区别

| 改进项 | 说明 |
|--------|------|
| SourceMod 1.12/1.13 支持 | 使用新版 SDK 接口和 AMBuild 2.2+ |
| 64 位支持 | 完整支持 x86_64 架构（Linux/Windows） |
| C++17 标准 | 使用现代编译器标志，提升性能和安全性 |
| 崩溃修复 | 增加 NULL 指针检查、栈对齐修复、安全的 gamedata 解析 |
| GitHub Actions CI | 自动构建 4 种平台组合（Win/Linux × x86/x64） |
| Docker 支持 | 提供容器化构建环境，确保可复现的编译结果 |

---

## 快速开始

### 方式一：Docker 构建（推荐，最简单）

```bash
# 1. 克隆仓库
git clone https://github.com/DNA-styx/dodhooks.git
cd dodhooks

# 2. 设置依赖
chmod +x setup_dependencies.sh
./setup_dependencies.sh

# 3. 使用 Docker 构建（自动拉取构建容器）
# 32位
docker run --rm -v $(pwd):/work -w /work \
    ghcr.io/alliedmodders/build-containers/debian11-clang22:latest \
    bash -c "apt-get update && pip3 install git+https://github.com/alliedmodders/ambuild.git && \
    mkdir -p build && cd build && \
    python3 ../configure.py --sm-path ../sourcemod --mms-path ../mmsource \
    --target x86 --enable-optimize && ambuild"

# 64位
docker run --rm -v $(pwd):/work -w /work \
    ghcr.io/alliedmodders/build-containers/debian11-clang22:latest \
    bash -c "apt-get update && pip3 install git+https://github.com/alliedmodders/ambuild.git && \
    mkdir -p build64 && cd build64 && \
    python3 ../configure.py --sm-path ../sourcemod --mms-path ../mmsource \
    --target x86_64 --enable-optimize && ambuild"
```

### 方式二：Linux 本地构建

```bash
# 1. 安装依赖
sudo apt-get update
sudo apt-get install -y build-essential clang-22 python3 python3-pip git
pip3 install --upgrade git+https://github.com/alliedmodders/ambuild.git

# 2. 克隆仓库和依赖
git clone https://github.com/DNA-styx/dodhooks.git
cd dodhooks
./setup_dependencies.sh

# 3. 配置和构建
mkdir build && cd build
python3 ../configure.py \
    --sm-path ../sourcemod \
    --mms-path ../mmsource \
    --target x86_64 \
    --enable-optimize
ambuild

# 输出在 build/package/ 目录
```

### 方式三：Windows 本地构建

```powershell
# 1. 安装依赖
#    - Visual Studio 2019+（含 C++ 桌面开发工作负载）
#    - Python 3.12+（从 python.org 下载）
#    - Git（从 git-scm.com 下载）

# 2. 安装 AMBuild
python -m pip install --upgrade git+https://github.com/alliedmodders/ambuild.git

# 3. 克隆仓库
git clone https://github.com/DNA-styx/dodhooks.git
cd dodhooks

# 4. 设置依赖
git clone --depth 1 -b 1.12-dev https://github.com/alliedmodders/metamod-source.git mmsource
git clone --depth 1 -b 1.12-dev https://github.com/alliedmodders/sourcemod.git sourcemod

# 5. 构建（在 VS 开发者命令提示符中运行）
# 32位：使用 "x86 Native Tools Command Prompt"
mkdir build && cd build
python ..\configure.py --sm-path ..\sourcemod --mms-path ..\mmsource --target x86 --enable-optimize
ambuild

# 64位：使用 "x64 Native Tools Command Prompt"
mkdir build64 && cd build64
python ..\configure.py --sm-path ..\sourcemod --mms-path ..\mmsource --target x86_64 --enable-optimize
ambuild
```

---

## 安装到服务器

将 `build/package/` 目录下的文件复制到游戏服务器根目录：

```
your-server/
└── addons/
    └── sourcemod/
        ├── extensions/
        │   ├── dodhooks.ext.dll         (Windows 32位)
        │   ├── dodhooks.ext.so          (Linux 32位)
        │   └── x64/
        │       ├── dodhooks.ext.dll     (Windows 64位)
        │       └── dodhooks.ext.so     (Linux 64位)
        └── gamedata/
            └── dodhooks.txt
```

---

## 项目结构

```
dodhooks/
├── .github/
│   └── workflows/
│       └── ci.yml              # GitHub Actions CI 配置
├── sourcemod/
│   ├── gamedata/
│   │   └── dodhooks.txt       # 游戏数据签名文件
│   ├── public/
│   │   └── README.md         # SourceMod 公共头文件说明
│   └── scripting/
│       ├── include/
│       │   └── dodhooks.inc   # SourcePawn 插件头文件
│       └── dodhooks_example.sp # 示例插件
├── .gitignore
├── .gitmodules               # Git 子模块配置
├── AMBuildScript             # AMBuild 主配置脚本
├── AMBuilder                 # AMBuild 源文件列表
├── Dockerfile                # Docker 构建镜像
├── LICENSE                   # GPL v2 许可证
├── PackageScript             # 打包脚本
├── README.md                 # 英文文档
├── README_zh.md             # 本文件 - 中文文档
├── build.sh                  # Linux 一键构建脚本
├── build.bat                 # Windows 一键构建脚本
├── configure.py              # 配置脚本（AMBuild 入口）
├── docker-compose.yml        # Docker Compose 配置
├── setup_dependencies.sh     # 依赖设置脚本
├── extension.h               # 扩展头文件
├── extension.cpp             # 扩展主实现
├── natives.h                 # 原生函数声明
├── natives.cpp               # 原生函数实现
├── vglobals.h               # Valve 全局变量接口
├── vglobals.cpp             # Valve 全局变量实现
└── smsdk_config.h           # 扩展配置（名称、版本等）
```

---

## 常见问题

### Q: 构建时报错 "Could not find a source copy of SourceMod"
A: 确保已运行 `setup_dependencies.sh` 或手动克隆了 SourceMod 到 `sourcemod/` 目录。

### Q: 64 位构建后服务器崩溃
A: 检查 gamedata 文件中的签名是否匹配你的游戏版本。64 位下的函数签名与 32 位不同。

### Q: 如何切换 SourceMod 版本？
A: 修改 `setup_dependencies.sh` 中的分支参数，或在克隆时指定分支：
```bash
git clone --depth 1 -b master https://github.com/alliedmodders/sourcemod.git sourcemod
```

### Q: Windows 下提示 "ambuild 不是内部命令"
A: 确保 Python Scripts 目录在 PATH 中，或使用完整路径：
```powershell
python -m ambuild
```

---

## 许可证

GPL v2 - 详见 [LICENSE](LICENSE) 文件。
