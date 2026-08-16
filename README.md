# DODHooks

> **SourceMod Extension with Detours & Natives for Day of Defeat: Source**

[![CI](https://github.com/kittenks/dodhooks-modified/workflows/CI/badge.svg)](https://github.com/kittenks/dodhooks-modified/actions)

## About

DODHooks is a SourceMod extension for **Day of Defeat: Source** that provides:

- **Detours** (hooks) for key game functions: voice commands, class joining, helmet popping, respawning, wave time, winning team, round state, player state, and bomb target state.
- **Natives** for controlling player classes, control point icons, round timers, and game rules from SourcePawn plugins.
- **Forwards** (hooks) that allow plugins to intercept and modify game events.

This version is a maintained fork that:

- Supports **SourceMod 1.12 and 1.13**
- Supports **Metamod:Source 1.12 and 1.13**
- Compiles for **both 32-bit (x86) and 64-bit (x86_64)** architectures
- Works on **Windows and Linux**
- Uses the **latest AMBuild 2.x** build system
- Fixes server crash issues present in older versions
- Uses modern C++17 compiler flags

## Requirements

| Dependency | Version | Notes |
|------------|---------|-------|
| SourceMod | 1.12 / 1.13 | Source code required for building |
| Metamod:Source | 1.12 / 1.13 | Source code required for building |
| AMBuild | 2.2+ | Python-based build system |
| Python | 3.8+ | Required for AMBuild |
| Compiler | GCC 9+ / Clang 10+ / MSVC 2019+ | C++17 support required |

## Build Dependencies

### Linux
```bash
# Debian/Ubuntu
sudo apt-get update
sudo apt-get install -y build-essential clang-22 python3 python3-pip git

# Install AMBuild
pip3 install --upgrade git+https://github.com/alliedmodders/ambuild.git
```

### Windows
```powershell
# Install Python 3.12+ from python.org
# Install Visual Studio 2019+ (Community Edition is fine)
# Install Git from git-scm.com

# Install AMBuild
python -m pip install --upgrade git+https://github.com/alliedmodders/ambuild.git
```

## Building

### Quick Start (Linux)

```bash
# Clone the repository
git clone https://github.com/kittenks/dodhooks-modified.git
cd dodhooks-modified

# Clone dependencies
git clone --depth 1 --recurse-submodules -b 1.12-dev https://github.com/alliedmodders/metamod-source.git mmsource
git clone --depth 1 --recurse-submodules -b 1.12-dev https://github.com/alliedmodders/sourcemod.git sourcemod

# Configure (32-bit)
mkdir build && cd build
python3 ../configure.py \
    --sm-path ../sourcemod \
    --mms-path ../mmsource \
    --target x86 \
    --enable-optimize

# Build
ambuild

# Output will be in build/package/
```

### 64-bit Build (Linux)

```bash
mkdir build64 && cd build64
python3 ../configure.py \
    --sm-path ../sourcemod \
    --mms-path ../mmsource \
    --target x86_64 \
    --enable-optimize
ambuild
```

### Windows Build

```powershell
# Open "Developer Command Prompt for VS" or run vcvars32.bat

git clone https://github.com/kittenks/dodhooks-modified.git
cd dodhooks-modified

git clone --depth 1 --recurse-submodules -b 1.12-dev https://github.com/alliedmodders/metamod-source.git mmsource
git clone --depth 1 --recurse-submodules -b 1.12-dev https://github.com/alliedmodders/sourcemod.git sourcemod

# 32-bit build
mkdir build && cd build
python ..\configure.py `
    --sm-path ..\sourcemod `
    --mms-path ..\mmsource `
    --target x86 `
    --enable-optimize
ambuild

# 64-bit build (use "x64 Native Tools Command Prompt")
mkdir build64 && cd build64
python ..\configure.py `
    --sm-path ..\sourcemod `
    --mms-path ..\mmsource `
    --target x86_64 `
    --enable-optimize
ambuild
```

### Generate Visual Studio Project (Windows)

```powershell
python ..\configure.py `
    --sm-path ..\sourcemod `
    --mms-path ..\mmsource `
    --target x86 `
    --enable-optimize `
    --gen=vs
```

## Docker Build

A Dockerfile is provided for easy Linux builds:

```bash
# Build the container
docker build -t dodhooks-builder .

# 32-bit build
docker run --rm -v $(pwd):/work -w /work dodhooks-builder \
    bash -c "mkdir -p build && cd build && \
    python3 ../configure.py --sm-path /sourcemod --mms-path /mmsource \
    --target x86 --enable-optimize && ambuild"

# 64-bit build
docker run --rm -v $(pwd):/work -w /work dodhooks-builder \
    bash -c "mkdir -p build && cd build && \
    python3 ../configure.py --sm-path /sourcemod --mms-path /mmsource \
    --target x86_64 --enable-optimize && ambuild"
```

Or with docker-compose:

```bash
# Build the image
docker-compose build

# 32-bit build
docker-compose run --rm dodhooks-builder \
    bash -c "mkdir -p build && cd build && \
    python3 ../configure.py --sm-path /sourcemod --mms-path /mmsource \
    --target x86 --enable-optimize && ambuild"

# 64-bit build
docker-compose run --rm dodhooks-builder \
    bash -c "mkdir -p build && cd build && \
    python3 ../configure.py --sm-path /sourcemod --mms-path /mmsource \
    --target x86_64 --enable-optimize && ambuild"
```

## Installation

After building, copy the contents of `build/package/` to your game server's root directory:

```
addons/
└── sourcemod/
    ├── extensions/
    │   ├── dodhooks.ext.dll        (Windows 32-bit)
    │   ├── dodhooks.ext.so         (Linux 32-bit)
    │   └── x64/
    │       ├── dodhooks.ext.dll    (Windows 64-bit)
    │       └── dodhooks.ext.so    (Linux 64-bit)
    ├── gamedata/
    │   └── dodhooks.txt
    └── scripting/
        └── include/
            └── dodhooks.inc
```

## Available Natives

| Native | Description |
|--------|-------------|
| `DOD_GetPlayerClass(client)` | Get a player's current class |
| `DOD_SetPlayerClass(client, class)` | Set a player's current class |
| `DOD_GetDesiredPlayerClass(client)` | Get desired player class |
| `DOD_SetDesiredPlayerClass(client, class)` | Set desired player class |
| `DOD_PopHelmet(client, velocity[3], origin[3])` | Force a helmet to pop off |
| `DOD_SetNumControlPoints(num)` | Set number of control points |
| `DOD_PrecacheCPIcon(material)` | Precache a CP icon material |
| `DOD_SetCPIcons(index, ...)` | Set icons for a control point |
| `DOD_SetCPVisible(index, visible)` | Show/hide a control point |
| `DOD_PauseTimer(timer)` | Pause a round timer |
| `DOD_ResumeTimer(timer)` | Resume a round timer |
| `DOD_SetTimeRemaining(timer, seconds)` | Set timer remaining time |
| `DOD_GetTimeRemaining(timer)` | Get timer remaining time |
| `DOD_RespawnPlayer(client, useClass)` | Force respawn a player |
| `DOD_AddWaveTime(team, delay)` | Add wave time for a team |
| `DOD_SetWinningTeam(team)` | Set the winning team |
| `DOD_SetRoundState(state)` | Set the round state |
| `DOD_SetPlayerState(client, state)` | Set a player's state |
| `DOD_SetBombTargetState(entity, state)` | Set bomb target state |

## Available Forwards (Hooks)

| Forward | Description |
|---------|-------------|
| `OnVoiceCommand(client, &voiceCommand)` | Called when a voice command is used |
| `OnJoinClass(client, &playerClass)` | Called when a player joins a class |
| `OnPopHelmet(client, velocity[3], origin[3])` | Called when a helmet pops off |
| `OnPlayerRespawn(client)` | Called when a player is about to respawn |
| `OnAddWaveTime(team, &delay)` | Called when wave time is added |
| `OnSetWinningTeam(team)` | Called when the winning team is set |
| `OnEnterRoundState(&roundState)` | Called when round state changes |
| `OnEnterPlayerState(client, &playerState)` | Called when player state changes |
| `OnEnterBombTargetState(entity, &bombState)` | Called when bomb target state changes |

## Changes from Original

- **SourceMod 1.12/1.13 compatibility** - Updated APIs and build system
- **64-bit support** - Compiles and runs on 64-bit servers
- **Modern C++17** - Updated compiler flags and standards
- **Fixed crashes** - Addressed several server crash scenarios:
  - NULL pointer checks in detour callbacks
  - Proper stack alignment for 64-bit ThisCall conventions
  - Safer gamedata signature resolution with better error messages
  - Protected against invalid entity references
- **Improved error handling** - Better error messages for missing gamedata or signatures
- **GitHub Actions CI** - Automated builds for 4 platforms (Win/Linux x86/x64)
- **Docker support** - Reproducible builds via containerization

## License

GPL v2 - See [LICENSE](LICENSE) for details.

## Credits

- **Andersso** - Original author
- **ChesterSmitty** - Previous maintainer
- **Apfelwurm** - CI improvements
- **DNA-styx** - Gamedata file
- **Kittenks** - Upstream maintainer (1.12/1.13 updates)
- **AlliedModders** - SourceMod, Metamod:Source, AMBuild
