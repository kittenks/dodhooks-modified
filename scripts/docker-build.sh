#!/bin/bash
# Docker 容器内编译脚本
# 由 Dockerfile.linux 的 CMD 调用

set -e

ARCH="${BUILD_ARCH:-x86_64}"
WORKSPACE="/workspace"

echo "=========================================="
echo "  DoDHooks Docker Build"
echo "  Arch: $ARCH"
echo "=========================================="

# 克隆依赖
echo "[1/4] Cloning dependencies..."
cd "$WORKSPACE"
git clone --depth 1 -b 1.12-dev https://github.com/alliedmodders/sourcemod.git sourcemod 2>/dev/null || true
git clone --depth 1 -b 1.11-dev https://github.com/alliedmodders/metamod-source.git mmsource 2>/dev/null || true
git clone --depth 1 -b dods https://github.com/alliedmodders/hl2sdk.git hl2sdk-dods 2>/dev/null || true

# Configure
echo "[2/4] Configuring..."
cd "$WORKSPACE/dodhooks"
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
echo "[3/4] Building..."
ambuild

# Package
echo "[4/4] Packaging..."
PKG_DIR="$WORKSPACE/package-$ARCH"
mkdir -p "$PKG_DIR"
cp -r "$WORKSPACE/dodhooks/build/package/"* "$PKG_DIR/"

echo ""
echo "=========================================="
echo "  BUILD SUCCESS"
echo "  Package: $PKG_DIR"
echo "=========================================="
ls -la "$PKG_DIR"
