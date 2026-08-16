#!/bin/bash
# DoDHooks Linux 本地编译脚本
# 用法: ./scripts/build-linux.sh [x86|x86_64]

set -e

ARCH="${1:-x86_64}"
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(dirname "$SCRIPT_DIR")"
WORKSPACE="$(dirname "$ROOT_DIR")"

echo "=========================================="
echo "  DoDHooks Linux Build"
echo "  Arch: $ARCH"
echo "=========================================="

# 检查依赖
command -v python3 >/dev/null 2>&1 || { echo "ERROR: python3 not found"; exit 1; }

# 安装 AMBuild
echo "[1/5] Installing AMBuild 2.2.0..."
pip install "AMBuild==2.2.0" 2>/dev/null || pip3 install "AMBuild==2.2.0"

# 检查/克隆依赖
echo "[2/5] Checking dependencies..."

if [ ! -d "$WORKSPACE/sourcemod" ]; then
    echo "  Cloning SourceMod..."
    git clone --depth 1 -b 1.12-dev https://github.com/alliedmodders/sourcemod.git "$WORKSPACE/sourcemod"
fi

if [ ! -d "$WORKSPACE/mmsource" ]; then
    echo "  Cloning Metamod:Source..."
    git clone --depth 1 -b 1.11-dev https://github.com/alliedmodders/metamod-source.git "$WORKSPACE/mmsource"
fi

if [ ! -d "$WORKSPACE/hl2sdk-dods" ]; then
    echo "  Cloning hl2sdk (dods)..."
    git clone --depth 1 -b dods https://github.com/alliedmodders/hl2sdk.git "$WORKSPACE/hl2sdk-dods"
fi

# 安装 32 位依赖
if [ "$ARCH" = "x86" ]; then
    echo "[3/5] Installing multilib deps..."
    sudo dpkg --add-architecture i386
    sudo apt-get update
    sudo apt-get install -y --no-install-recommends \
        gcc-multilib g++-multilib lib32stdc++6 \
        zlib1g-dev zlib1g-dev:i386 libc6-dev-i386 linux-libc-dev:i386
fi

# Configure
echo "[4/5] Configuring..."
cd "$ROOT_DIR"
rm -rf build
mkdir build
cd build

if [ "$ARCH" = "x86" ]; then
    export CC=gcc
    export CXX=g++
else
    export CC=clang
    export CXX=clang++
fi

python3 ../configure.py \
    --targets=$ARCH \
    --enable-optimize \
    --sm-path=$WORKSPACE/sourcemod \
    --mms-path=$WORKSPACE/mmsource \
    --hl2sdk-root=$WORKSPACE \
    --sdks=dod

# Build
echo "[5/5] Building..."
ambuild

echo ""
echo "=========================================="
echo "  BUILD SUCCESS"
echo "  Output: $ROOT_DIR/build/package/"
echo "=========================================="
ls -la "$ROOT_DIR/build/package/" 2>/dev/null || echo "(package dir not found, check build output)"
