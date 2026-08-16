# DoDHooks

Day of Defeat: Source 的 SourceMod 扩展，提供钩子（Hooks）和原生函数（Natives），
兼容 **SourceMod 1.12 / 1.13**，支持 **Windows + Linux × x86 + x86_64**。

## 功能

### Detour Hooks（检测/拦截）
- `OnVoiceCommand` — 语音命令
- `OnJoinClass` — 职业切换
- `OnPopHelmet` — 头盔弹出
- `OnPlayerRespawn` — 重生
- `OnAddWaveTime` — 波次时间
- `OnSetWinningTeam` — 胜利队伍
- `OnEnterRoundState` — 回合状态
- `OnEnterPlayerState` — 玩家状态
- `OnEnterBombTargetState` — 炸弹目标状态

### Natives（插件可调用的函数）
- 玩家职业查询/设置
- 控制点状态查询/修改
- 炸弹目标查询
- 计时器工具
- 平台检测 `DODHooks_GetPlatform()`

## 目录结构

```
dodhooks/
├── .github/workflows/build.yml   # GitHub Actions CI
├── docker/
│   ├── Dockerfile.linux         # Docker 编译环境
│   ├── Dockerfile.windows      # Docker Windows 交叉编译
│   └── docker-compose.yml      # 多架构编排
├── scripts/
│   ├── build-linux.sh          # Linux 本地编译
│   ├── build-windows.bat       # Windows 本地编译
│   ├── docker-build.sh        # Docker 容器内编译
│   └── package-release.sh     # 一键打包发布
├── sourcemod/
│   ├── gamedata/dodhooks.txt  # 游戏数据签名
│   ├── scripting/include/dodhooks.inc   # SourcePawn 头文件
│   └── scripting/dodhooks_example.sp  # 示例插件
├── extension.cpp / .h          # 扩展主逻辑
├── natives.cpp / .h            # Native 函数实现
├── vglobals.cpp / .h           # Valve 全局变量
├── smsdk_config.h             # SDK 配置
├── AMBuildScript              # AMBuild 构建脚本
├── AMBuilder                  # AMBuild 构建规则
├── configure.py               # 配置脚本
└── README.md
```

## 快速开始（编译）

### 方式一：GitHub Actions（推荐）

推送代码后自动编译，无需本地环境：

```bash
git push origin main
```

进入 GitHub → Actions 标签页，等待 4 个矩阵任务完成，下载 Artifacts。

**打 Tag 自动发 Release：**

```bash
git tag v2.0.0
git push origin v2.0.0
```

### 方式二：Docker（本地一键编译）

```bash
# 编译 x86_64
docker build -f docker/Dockerfile.linux -t dodhooks-build .
docker run --rm -v $(pwd):/workspace -e BUILD_ARCH=x86_64 dodhooks-build

# 编译 x86
docker run --rm -v $(pwd):/workspace -e BUILD_ARCH=x86 dodhooks-build
```

### 方式三：本地手动编译

#### Linux

```bash
chmod +x scripts/build-linux.sh
./scripts/build-linux.sh x86_64     # 或 x86
```

#### Windows

```cmd
scripts\build-windows.bat x86_64    :: 或 x86
```

需要：Python 3.11 + Visual Studio 2022 + Git。

## 安装到服务器

将编译产物解压后放到 `addons/sourcemod/` 对应目录：

```
addons/sourcemod/
├── extensions/
│   └── dodhooks.ext.dll / dodhooks.ext.so
├── gamedata/
│   └── dodhooks.txt
├── scripting/include/
│   └── dodhooks.inc          (编译插件用)
└── plugins/
    └── dodhooks_example.smx (示例)
```

## 依赖版本（已验证）

| 组件 | 版本 | 说明 |
|------|------|------|
| SourceMod | 1.12-dev | 兼容 1.13 |
| Metamod:Source | 1.12-dev | 兼容 1.13 |
| hl2sdk | dods 分支 | DoD:S SDK |
| AMBuild | master (源码安装) | 从 GitHub checkout 安装 |
| Python | 3.11 | 3.12+ 未验证 |
| 编译器 | GCC/Clang/MSVC 2022 | — |

## AMBuild 安装说明

本项目使用 AMBuild 作为构建系统。由于 PyPI 上的版本可能不兼容，
**推荐从源码安装**：

```bash
git clone https://github.com/alliedmodders/ambuild
pip install ./ambuild
```

## API 速览

```pawn
// 获取当前平台
DODPlatform:DODHooks_GetPlatform();

// 职业切换钩子
forward Action:OnJoinClass(client, &DODClass:class);

// 语音命令钩子
forward Action:OnVoiceCommand(client, &voiceCommand);

// 控制点
native DODHooks_GetCapPointState(controlpoint);
native DODHooks_SetCapPointState(controlpoint, state);

// 玩家职业
native DODClass:DOD_GetPlayerClass(client);
native DODHooks_SetPlayerClass(client, DODClass:class);
```

完整 API 见 `sourcemod/scripting/include/dodhooks.inc`。

## 故障排查

| 问题 | 解决 |
|------|------|
| `ImportError: cannot import 'runplugin'` | AMBuild 版本不对，从源码重新安装最新 master |
| `configure did not produce .ambuild2/vars` | configure 失败，检查依赖路径和 Python 版本 |
| `unrecognized arguments: --outdir` | configure.py 不支持 `--outdir`，删除该参数 |
| 运行时 `Failed to load extension` | gamedata 签名不匹配，更新 `dodhooks.txt` |
| `No matching distribution for AMBuild` | 不要从 PyPI 安装，改用 `pip install ./ambuild`（源码） |

## License

GPL v2.0
