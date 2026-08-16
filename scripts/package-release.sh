#!/bin/bash
# 一键打包发布脚本
# 用法: ./scripts/package-release.sh [version]
# 示例: ./scripts/package-release.sh 2.0.0

set -e

VERSION="${1:-2.0.0}"
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(dirname "$SCRIPT_DIR")"
DIST_DIR="$ROOT_DIR/dist"
TIMESTAMP=$(date +%Y%m%d-%H%M%S)

echo "=========================================="
echo "  DoDHooks Release Packager"
echo "  Version: $VERSION"
echo "=========================================="

# 清理旧产物
rm -rf "$DIST_DIR"
mkdir -p "$DIST_DIR"

# 创建源码包
echo ""
echo "[1/4] Creating source package..."
SRC_NAME="dodhooks-$VERSION-source"
SRC_DIR="$DIST_DIR/$SRC_NAME"
mkdir -p "$SRC_DIR"

# 复制源码文件（排除不需要的）
cd "$ROOT_DIR"
rsync -av --exclude='.git' \
    --exclude='dist' \
    --exclude='__pycache__' \
    --exclude='*.pyc' \
    --exclude='build' \
    --exclude='.ambuild2' \
    --exclude='package-*' \
    --exclude='.vscode' \
    --exclude='.idea' \
    . "$SRC_DIR/"

# 创建源码 zip
cd "$DIST_DIR"
zip -r "${SRC_NAME}.zip" "$SRC_NAME/" -x "*.git*" "*.DS_Store*"
echo "  ✅ ${SRC_NAME}.zip"

# 编译 Linux x86_64
echo ""
echo "[2/4] Building Linux x86_64..."
mkdir -p "$ROOT_DIR/build-linux-x64"
cd "$ROOT_DIR/build-linux-x64"
cmake -DCMAKE_BUILD_TYPE=Release \
    -DSM_PATH="$ROOT_DIR/../sourcemod" \
    -DMMS_PATH="$ROOT_DIR/../mmsource" \
    -DHL2SDK_ROOT="$ROOT_DIR/.." \
    -DSDKS=dod \
    "$ROOT_DIR" 2>/dev/null || true

# 如果 cmake 不行，用 ambuild 方式
cd "$ROOT_DIR"
if [ -f "configure.py" ]; then
    rm -rf build
    mkdir build && cd build
    python3 ../configure.py \
        --targets=x86_64 \
        --enable-optimize \
        --sm-path="$ROOT_DIR/../sourcemod" \
        --mms-path="$ROOT_DIR/../mmsource" \
        --hl2sdk-root="$ROOT_DIR/.." \
        --sdks=dod
    ambuild
    cd "$ROOT_DIR"
fi

# 编译 Linux x86
echo ""
echo "[3/4] Building Linux x86..."
export CC=gcc
export CXX=g++
cd "$ROOT_DIR"
rm -rf build
mkdir build && cd build
python3 ../configure.py \
    --targets=x86 \
    --enable-optimize \
    --sm-path="$ROOT_DIR/../sourcemod" \
    --mms-path="$ROOT_DIR/../mmsource" \
    --hl2sdk-root="$ROOT_DIR/.." \
    --sdks=dod
ambuild
cd "$ROOT_DIR"

# 打包各平台产物
echo ""
echo "[4/4] Packaging binaries..."

# Linux x86_64
if [ -d "$ROOT_DIR/build/package" ]; then
    PKG_NAME="dodhooks-$VERSION-linux-x86_64"
    PKG_DIR="$DIST_DIR/$PKG_NAME"
    mkdir -p "$PKG_DIR"
    cp -r "$ROOT_DIR/build/package/"* "$PKG_DIR/"
    cd "$DIST_DIR"
    zip -r "${PKG_NAME}.zip" "$PKG_NAME/"
    echo "  ✅ ${PKG_NAME}.zip"
fi

# 重新编 x86 拿产物
cd "$ROOT_DIR"
rm -rf build
mkdir build && cd build
python3 ../configure.py \
    --targets=x86 \
    --enable-optimize \
    --sm-path="$ROOT_DIR/../sourcemod" \
    --mms-path="$ROOT_DIR/../mmsource" \
    --hl2sdk-root="$ROOT_DIR/.." \
    --sdks=dod
ambuild
cd "$ROOT_DIR"

if [ -d "$ROOT_DIR/build/package" ]; then
    PKG_NAME="dodhooks-$VERSION-linux-x86"
    PKG_DIR="$DIST_DIR/$PKG_NAME"
    mkdir -p "$PKG_DIR"
    cp -r "$ROOT_DIR/build/package/"* "$PKG_DIR/"
    cd "$DIST_DIR"
    zip -r "${PKG_NAME}.zip" "$PKG_NAME/"
    echo "  ✅ ${PKG_NAME}.zip"
fi

# 创建完整发布包（含所有平台说明）
echo ""
echo "Creating full release bundle..."
BUNDLE_NAME="dodhooks-$VERSION-full"
BUNDLE_DIR="$DIST_DIR/$BUNDLE_NAME"
mkdir -p "$BUNDLE_DIR"

# 复制源码
cp -r "$SRC_DIR" "$BUNDLE_DIR/source/"

# 复制文档
cp "$ROOT_DIR/README.md" "$BUNDLE_DIR/"
cp "$ROOT_DIR/LICENSE" "$BUNDLE_DIR/" 2>/dev/null || echo "GPL v3" > "$BUNDLE_DIR/LICENSE"

# 复制构建脚本
mkdir -p "$BUNDLE_DIR/scripts"
cp "$ROOT_DIR/scripts/"*.sh "$BUNDLE_DIR/scripts/" 2>/dev/null || true
cp "$ROOT_DIR/scripts/"*.bat "$BUNDLE_DIR/scripts/" 2>/dev/null || true

# 复制 Docker 文件
mkdir -p "$BUNDLE_DIR/docker"
cp "$ROOT_DIR/docker/"* "$BUNDLE_DIR/docker/" 2>/dev/null || true

# 复制 CI 配置
mkdir -p "$BUNDLE_DIR/.github/workflows"
cp "$ROOT_DIR/.github/workflows/"*.yml "$BUNDLE_DIR/.github/workflows/" 2>/dev/null || true

cd "$DIST_DIR"
zip -r "${BUNDLE_NAME}.zip" "$BUNDLE_NAME/"

echo ""
echo "=========================================="
echo "  PACKAGING COMPLETE"
echo "=========================================="
ls -lh "$DIST_DIR/"*.zip
echo ""
echo "上传以下文件到 GitHub Releases:"
echo "  - ${SRC_NAME}.zip"
echo "  - dodhooks-$VERSION-linux-x86_64.zip"
echo "  - dodhooks-$VERSION-linux-x86.zip"
echo "  - ${BUNDLE_NAME}.zip"
echo ""
echo "Windows 版本请在 GitHub Actions Artifacts 中下载"
echo "或使用 Docker: docker-compose -f docker/docker-compose.yml up"
