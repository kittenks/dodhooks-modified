# DoDHooks - SourceMod Extension for Day of Defeat: Source

[![Build Status](https://img.shields.io/badge/build-passing-green)]()
[![SourceMod](https://img.shields.io/badge/SourceMod-1.12%20%7C%201.13-blue)]()
[![License](https://img.shields.io/badge/license-GPL--2.0-orange)]()

> **DoDHooks** 是一个 SourceMod 扩展，为《胜利之日：起源》(Day of Defeat: Source) 提供 Detour 钩子和 Native 函数，让插件开发者能够拦截和修改游戏核心行为。

## ✨ 特性

- 🎯 **9 个 Detour 钩子** - 拦截语音命令、职业切换、头盔弹出、重生、波次时间、胜负判定、回合状态、玩家状态、炸弹目标状态
- 🔧 **18 个 Native 函数** - 获取/设置玩家职业、控制点图标、计时器、重生、回合状态等
- 🌐 **全平台支持** - Windows (x86/x64)、Linux (x86/x64)
- 📦 **SourceMod 1.12/1.13** - 使用最新 AMBuild 2.2 构建系统
- 🐳 **Docker 编译** - 一键容器化编译
- 🔄 **GitHub Actions CI** - 自动构建所有平台二进制文件

## 📋 系统要求

| 组件 | 版本要求 |
|--------|------------|
| SourceMod | 1.12.x 或 1.13.x |
| Metamod:Source | 1.12.x 或 1.13.x |
| Day of Defeat: Source | 最新版 |
| Python | 3.8+ |
| AMBuild | 2.2+ |

### 编译器要求

| 平台 | 推荐编译器 | 最低版本 |
|--------|-------------|----------|
| Windows x86/x64 | MSVC 2019 (v142) | MSVC 2017 |
| Linux x86 | GCC 9 (multilib) | GCC 8 |
| Linux x64 | GCC 9 / Clang 16+ | GCC 8 / Clang 10 |
| Linux x86 | Clang 16+ (multilib) | Clang 10 |

## 🚀 快速安装

### 从 Release 安装

1. 从 [Releases](https://github.com/DODSLeague/dodhooks/releases) 下载对应平台的压缩包
2. 解压到服务器 `addons/` 目录
3. 重启服务器或执行 `sm plugins refresh`

### 目录结构

```
addons/
└── sourcemod/
    ├── extensions/
    │   ├── dodhooks.ext.dll        (Windows)
    │   ├── dodhooks.ext.so         (Linux)
    │   └── dodhooks.autoload
    ├── gamedata/
    │   └── dodhooks.txt
    └── scripting/
        └── include/
            └── dodhooks.inc
```

## 🔨 编译指南

### 方法一：GitHub Actions（推荐）

最简单的编译方式 - 无需本地环境：

1. Fork 本仓库
2. 在 GitHub 网页上进入 **Actions** 标签页
3. 选择 **Build DoDHooks** 工作流
4. 点击 **Run workflow**
5. 等待构建完成，下载 Artifacts

构建矩阵：

| 平台 | 架构 | 编译器 |
|--------|--------|----------|
| Windows | x86 (32-bit) | MSVC 2019 |
| Windows | x86_64 (64-bit) | MSVC 2019 |
| Linux | x86 (32-bit) | GCC 9 |
| Linux | x86_64 (64-bit) | GCC 9 |

### 方法二：Docker 编译（Linux）

```bash
# 克隆仓库
git clone https://github.com/DODSLeague/dodhooks.git
cd dodhooks

# 使用 Docker 编译（自动构建 x86 + x86_64）
chmod +x scripts/docker-build.sh
./scripts/docker-build.sh

# 输出文件在 output/ 目录
ls output/
# dodhooks-linux-x86.zip
# dodhooks-linux-x86_64.zip
```

### 方法三：手动编译

#### Linux

```bash
# 安装依赖 (Ubuntu/Debian)
sudo apt-get update
sudo apt-get install -y python3 python3-pip git \
    gcc-9 g++-9 zip

# 安装 AMBuild
python3 -m pip install ambuild

# 克隆依赖
git clone --depth 1 --branch 1.12-dev https://github.com/alliedmodders/sourcemod.git
git clone --depth 1 --branch 1.12-dev https://github.com/alliedmodders/metamod-source.git
git clone --depth 1 --branch master https://github.com/alliedmodders/hl2sdk.git hl2sdk-dods

# 编译 x86_64
mkdir build && cd build
python3 ../configure.py \
    --sm-path=../sourcemod \
    --mms-path=../metamod-source \
    --hl2sdk-root=.. \
    --enable-optimize \
    --targets=x86_64
ambuild

# 输出在 build/package/ 目录
```

#### Windows

```cmd
REM 使用 Visual Studio Developer Command Prompt
REM 或运行 vcvarsall.bat

REM 安装 AMBuild
python -m pip install ambuild

REM 克隆依赖（同 Linux）

REM 编译
mkdir build && cd build
python ..\configure.py ^
    --sm-path=..\sourcemod ^
    --mms-path=..\metamod-source ^
    --hl2sdk-root=.. ^
    --enable-optimize ^
    --targets=x86_64
ambuild
```

## 📝 API 参考

### Detour 钩子（Forwards）

| 钩子 | 触发时机 | 参数 |
|-------|----------|--------|
| `OnVoiceCommand` | 玩家使用语音命令 | `client`, `&voiceCommand` |
| `OnJoinClass` | 玩家选择职业 | `client`, `&class` |
| `OnPopHelmet` | 头盔被击落 | `client`, `velocity[3]`, `origin[3]` |
| `OnPlayerRespawn` | 玩家即将重生 | `client` |
| `OnAddWaveTime` | 添加波次时间 | `team`, `&delay` |
| `OnSetWinningTeam` | 设置获胜队伍 | `team` |
| `OnEnterRoundState` | 回合状态变化 | `&state` |
| `OnEnterPlayerState` | 玩家状态变化 | `client`, `&state` |
| `OnEnterBombTargetState` | 炸弹目标状态变化 | `entity`, `&state` |

### Native 函数

```sourcepawn
// 职业管理
int DOD_GetPlayerClass(int client);
bool DOD_SetPlayerClass(int client, int class);
int DOD_GetDesiredPlayerClass(int client);
bool DOD_SetDesiredPlayerClass(int client, int class);

// 重生
bool DOD_RespawnPlayer(int client, bool forceRespawn = true);

// 回合控制
void DOD_SetWinningTeam(int team);
bool DOD_SetRoundState(int state);
bool DOD_AddWaveTime(int team, float delay);

// 计时器
bool DOD_PauseTimer(int timerEnt);
bool DOD_ResumeTimer(int timerEnt);
bool DOD_SetTimeRemaining(int timerEnt, float time);
float DOD_GetTimeRemaining(int timerEnt);

// 控制点
bool DOD_SetNumControlPoints(int numCPs);
int DOD_PrecacheCPIcon(const char[] material);
bool DOD_SetCPIcons(int cpIndex, int allies, int axis, int neutral, int timer, int bombed);
bool DOD_SetCPVisible(int cpIndex, bool visible);

// 炸弹目标
bool DOD_SetBombTargetState(int entity, int state);
```

## 🏗️ 项目结构

```
dodhooks/
├── .github/
│   └── workflows/
│       └── build.yml          # GitHub Actions CI 配置
├── docker/
│   ├── Dockerfile.linux      # Linux 编译容器
│   ├── Dockerfile.windows    # Windows 交叉编译参考
│   └── docker-compose.yml    # Docker Compose 配置
├── scripts/
│   ├── build-linux.sh       # Linux 编译脚本
│   ├── build-windows.bat     # Windows 编译脚本
│   └── docker-build.sh      # Docker 编译脚本
├── sourcemod/
│   ├── gamedata/
│   │   └── dodhooks.txt     # 游戏数据签名
│   └── scripting/
│       └── include/
│           └── dodhooks.inc  # SourcePawn 头文件
├── AMBuildScript              # AMBuild 主脚本
├── AMBuilder                 # 文件复制脚本
├── PackageScript             # 打包脚本
├── configure.py              # 配置脚本
├── extension.cpp             # 扩展主源文件
├── extension.h               # 扩展头文件
├── natives.cpp               # Native 实现
├── natives.h                 # Native 声明
├── vglobals.cpp              # Valve 全局变量
├── vglobals.h                # Valve 全局变量头文件
├── smsdk_config.h           # SDK 配置
└── README.md                 # 本文件
```

## 🔄 更新日志

### v2.0.0 (当前版本)
- ✅ 全面支持 SourceMod 1.12 / 1.13 新语法
- ✅ 新增 64 位 (x86_64) 支持
- ✅ 更新 AMBuild 2.2 构建系统
- ✅ 集成 SafetyHook（SourceMod 1.12+）
- ✅ 新增 GitHub Actions CI 工作流
- ✅ 新增 Docker 编译支持
- ✅ 更新 gamedata 签名（基于 DNA-styx/dodhooks）
- ✅ 修复 Linux 编译警告
- ✅ 新增 `OnAddWaveTime` 钩子
- ✅ 新增 `OnSetWinningTeam` 钩子

### v1.x (原版)
- 仅支持 Linux x86
- 基于 SourceMod 1.10
- 使用旧版 AMBuild 1.x

## 📄 许可证

GPL-2.0 License - 详见 [LICENSE](LICENSE) 文件。

## 🙏 致谢

- **Joakim Andersson (Andersso)** - 原始作者
- **ChesterSmitty** - 原始维护者
- **DNA-styx** - 更新 gamedata 签名
- **Apfelwurm** - CI 模板
- **AlliedModders** - SourceMod / Metamod:Source / HL2SDK
- **dronelektron** - sm-dod-hooks 插件参考
- **Kenzzer** - 64 位兼容性工作

## 🐛 问题反馈

请在 [GitHub Issues](https://github.com/DODSLeague/dodhooks/issues) 报告问题。
