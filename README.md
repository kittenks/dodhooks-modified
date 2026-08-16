# DoDHooks - Day of Defeat: Source Extension

[![Build Status](https://github.com/kittenks/dodhooks-modified/workflows/Build/badge.svg)](https://github.com/kittenks/dodhooks-modified/actions)

A SourceMod extension for **Day of Defeat: Source** that provides server-side hooks and natives for plugin developers.

## ✨ Features

- **Voice Command Hooks** - Intercept and modify voice chat commands
- **Class Change Hooks** - Block or modify player class switches
- **Helmet Pop Detection** - Detect when helmets are shot off
- **Spawn Hooks** - Pre and post spawn events
- **Control Point API** - Read/set capture point ownership and progress
- **Bomb Target API** - Track bomb planting, defusal, and timers
- **Round Timer Control** - Get and set round time
- **Player State Natives** - Team, class, weapon queries and modifications
- **Full 64-bit Support** - Works on both Windows and Linux, 32-bit and 64-bit

## 📋 Requirements

- **SourceMod** 1.12 or 1.13
- **Metamod:Source** 1.11 or 1.12
- **Day of Defeat: Source** dedicated server
- For compiling: see [Compilation Guide](#-compilation)

## 🚀 Installation

1. Download the latest release from the [Releases page](https://github.com/kittenks/dodhooks-modified/releases)
2. Extract to your `dod/addons/` directory
3. Load the extension:
   ```
   meta load addons/sourcemod/extensions/dodhooks.ext
   ```
4. Verify: `sm exts list` should show `[✓] DoDHooks`

## 📦 Package Structure

```
addons/sourcemod/
├── extensions/
│   └── dodhooks.ext.dll / dodhooks.ext.so
├── gamedata/
│   └── dodhooks.txt
├── scripting/
│   ├── include/
│   │   └── dodhooks.inc
│   └── dodhooks_example.sp
└── translations/
    └── dodhooks.phrases.txt
```

## 🔧 Compilation

### GitHub Actions (Recommended)

Push to your fork and GitHub Actions will automatically build for:
- ✅ Windows x86_64
- ✅ Linux x86_64

Download artifacts from the Actions tab.

### Linux (Docker)

```bash
# Build the container
docker build -f docker/Dockerfile.linux -t dodhooks-build .

# Run build, mounting your source
docker run --rm -v $(pwd):/src dodhooks-build
```

### Linux (Native)

```bash
# Install dependencies
sudo apt-get update
sudo apt-get install -y build-essential gcc-multilib g++-multilib \
    python3 python3-pip git clang

# Install AMBuild
pip3 install "AMBuild==2.2.0"

# Clone dependencies
git clone -b 1.12-dev https://github.com/alliedmodders/sourcemod
git clone -b 1.12-dev https://github.com/alliedmodders/metamod-source
git clone -b dods https://github.com/alliedmodders/hl2sdk hl2sdk-dods

# Build
chmod +x scripts/build-linux.sh
./scripts/build-linux.sh x86_64
```

### Windows (Native)

```cmd
:: Install Visual Studio 2022 with C++ workload
:: Install Python 3.11 from python.org

:: Install AMBuild
pip install "AMBuild==2.2.0"

:: Clone dependencies
git clone -b 1.12-dev https://github.com/alliedmodders/sourcemod
git clone -b 1.12-dev https://github.com/alliedmodders/metamod-source
git clone -b dods https://github.com/alliedmodders/hl2sdk hl2sdk-dods

:: Build (run from VS Developer Command Prompt)
scripts\build-windows.bat x86_64
```

## 📝 API Reference

### Natives

| Native | Description |
|--------|-------------|
| `DODHooks_GetPlayerClass(client)` | Get player's current class |
| `DODHooks_SetPlayerClass(client, classId)` | Change player's class |
| `DODHooks_GetPlayerTeam(client)` | Get player's team |
| `DODHooks_SetPlayerTeam(client, team)` | Move player to team |
| `DODHooks_GetPlayerWeapon(client)` | Get active weapon ID |
| `DODHooks_GivePlayerWeapon(client, weaponId)` | Give weapon to player |
| `DODHooks_RemovePlayerWeapon(client, weaponId)` | Remove weapon |
| `DODHooks_GetCapIndex(entity)` | Get control point index |
| `DODHooks_GetCapOwner(cpIndex)` | Get CP owner team |
| `DODHooks_SetCapOwner(cpIndex, team)` | Set CP owner team |
| `DODHooks_GetCapProgress(cpIndex)` | Get capture progress (0-100) |
| `DODHooks_GetBombTarget()` | Get active bomb target entity |
| `DODHooks_IsBombPlanted()` | Check if bomb is planted |
| `DODHooks_GetBombTimer()` | Get bomb timer seconds |
| `DODHooks_GetRoundTime()` | Get current round time |
| `DODHooks_SetRoundTime(seconds)` | Set round time |
| `DODHooks_IsPlayerSpawned(client)` | Check if player is alive/spawned |
| `DODHooks_GetPlatform()` | Get current platform constant |

### Forwards

| Forward | Description |
|---------|-------------|
| `DODHooks_OnVoiceCommand(client, commandId)` | Voice command fired |
| `DODHooks_OnClassChange(client, oldClass, newClass)` | Class changing |
| `DODHooks_OnHelmetPop(client, attacker, weaponId)` | Helmet popped off |
| `DODHooks_OnPreSpawn(client)` | Before player spawns |
| `DODHooks_OnPostSpawn(client)` | After player spawns |
| `DODHooks_OnTeleport(client, origin, angles, velocity)` | Player teleported |
| `DODHooks_OnCapControl(cpIndex, oldTeam, newTeam)` | CP changed hands |
| `DODHooks_OnBombPlant(client, target)` | Bomb planted |
| `DODHooks_OnBombDefuse(client, target)` | Bomb defused |
| `DODHooks_OnRoundStart()` | Round started |
| `DODHooks_OnRoundEnd(winner)` | Round ended |
| `DODHooks_OnPlayerDeath(victim, attacker, weaponId)` | Player died |

### Action Values

Return these from forwards to control behavior:

```sourcepawn
#define DODHOOK_ACTION_CONTINUE  0  // Allow the action
#define DODHOOK_ACTION_CHANGED   1  // Modified, use new values
#define DODHOOK_ACTION_HANDLED   2  // Block, but don't stop chain
#define DODHOOK_ACTION_STOP      3  // Block completely
```

## 🏗️ Project Structure

```
dodhooks/
├── .github/workflows/    # GitHub Actions CI
│   └── build.yml
├── docker/               # Docker build containers
│   ├── Dockerfile.linux
│   ├── Dockerfile.windows
│   └── docker-compose.yml
├── scripts/             # Local build scripts
│   ├── build-linux.sh
│   └── build-windows.bat
├── sourcemod/           # SourceMod-related files
│   ├── gamedata/
│   │   └── dodhooks.txt
│   ├── scripting/
│   │   ├── include/
│   │   │   └── dodhooks.inc
│   │   └── dodhooks_example.sp
│   └── translations/
│       └── dodhooks.phrases.txt
├── extension.h/cpp      # Main extension logic
├── natives.h/cpp        # SourcePawn natives
├── vglobals.h/cpp       # Valve global variable access
├── smsdk_config.h       # SDK configuration
├── configure.py         # AMBuild configure script
├── AMBuildScript        # AMBuild project definition
├── AMBuilder            # AMBuild build rules
├── PackageScript        # AMBuild packaging rules
└── README.md
```

## 🐛 Troubleshooting

### Extension fails to load

Check `addons/sourcemod/logs/errors.log` for details. Common issues:
- Wrong SM/MM version
- Missing or outdated gamedata (`dodhooks.txt`)
- Server not running Day of Defeat: Source

### Hooks not firing

Ensure the extension loaded successfully:
```
sm exts list
```
Verify gamedata offsets match your server version. Update `dodhooks.txt` if needed.

### Build fails on CI

Check the Actions tab for detailed logs. Common fixes:
- Ensure all submodules are checked out
- Verify Python 3.11 is being used
- Check that AMBuild 2.2.0 is installed

## 📄 License

GPL v3 - See LICENSE file for details.

## 🙏 Credits

- Original DoDHooks by [DODSLeague](https://github.com/DODSLeague/dodhooks)
- Updated gamedata by [DNA-styx](https://github.com/DNA-styx/dodhooks)
- SourceMod team for the SDK and build system
- AlliedModders community

## 🔗 Links

- [SourceMod](https://www.sourcemod.net/)
- [Metamod:Source](https://www.metamodsource.org/)
- [Day of Defeat: Source](https://store.steampowered.com/app/300/)
- [AMBuild Documentation](https://wiki.alliedmods.net/AMBuild)
