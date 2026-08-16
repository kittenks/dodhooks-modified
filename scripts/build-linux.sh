#!/bin/bash
# ============================================================
# DoDHooks - Linux Build Script
# For SourceMod 1.12 / 1.13
# Supports: x86 (32-bit) and x86_64 (64-bit)
# ============================================================

set -e

# ---- Configuration ----
SM_BRANCH="${SM_BRANCH:-1.12-dev}"
MMS_BRANCH="${MMS_BRANCH:-1.12-dev}"
HL2SDK_BRANCH="${HL2SDK_BRANCH:-master}"

# Default to x86_64, can be overridden
TARGET="${1:-x86_64}"

# Compiler selection
CC="${CC:-gcc-9}"
CXX="${CXX:-g++-9}"

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
DEPS="${ROOT}/deps"
BUILD="${ROOT}/build-${TARGET}"

echo "============================================================"
echo " DoDHooks Linux Build Script"
echo " Target: ${TARGET}"
echo " Compiler: ${CC} / ${CXX}"
echo "============================================================"
echo ""

# ---- Check dependencies ----
check_cmd() {
    if ! command -v "$1" >/dev/null 2>&1; then
        echo "[ERROR] $1 not found. Please install it first."
        exit 1
    fi
}

check_cmd python3
check_cmd git
check_cmd "${CC}"
check_cmd "${CXX}"

# Check for AMBuild
if ! python3 -c "import ambuild" 2>/dev/null; then
    echo "[INFO] Installing AMBuild..."
    python3 -m pip install --upgrade pip
    python3 -m pip install ambuild
fi

# ---- Install 32-bit libs if building x86 ----
if [ "${TARGET}" = "x86" ]; then
    echo "[INFO] Checking 32-bit libraries..."
    if ! dpkg --print-foreign-architectures 2>/dev/null | grep -q i386; then
        echo "[INFO] Adding i386 architecture..."
        sudo dpkg --add-architecture i386
        sudo apt-get update
    fi
    sudo apt-get install -y \
        gcc-9-multilib g++-9-multilib \
        lib32stdc++6 lib32z1-dev libc6-dev-i386 \
        linux-libc-dev:i386 2>/dev/null || true
fi

# ---- Setup directories ----
mkdir -p "${DEPS}"

# ---- Clone/Update SourceMod ----
cd "${DEPS}"
if [ ! -d "sourcemod" ]; then
    echo "[INFO] Cloning SourceMod..."
    git clone --depth 1 --branch "${SM_BRANCH}" https://github.com/alliedmodders/sourcemod.git
else
    echo "[INFO] Updating SourceMod..."
    cd sourcemod
    git fetch --depth 1 origin "${SM_BRANCH}"
    git checkout "${SM_BRANCH}"
    git pull origin "${SM_BRANCH}"
    cd ..
fi

# ---- Clone/Update Metamod:Source ----
if [ ! -d "metamod-source" ]; then
    echo "[INFO] Cloning Metamod:Source..."
    git clone --depth 1 --branch "${MMS_BRANCH}" https://github.com/alliedmodders/metamod-source.git
else
    echo "[INFO] Updating Metamod:Source..."
    cd metamod-source
    git fetch --depth 1 origin "${MMS_BRANCH}"
    git checkout "${MMS_BRANCH}"
    git pull origin "${MMS_BRANCH}"
    cd ..
fi

# ---- Clone/Update HL2SDK ----
if [ ! -d "hl2sdk-dods" ]; then
    echo "[INFO] Cloning HL2SDK..."
    git clone --depth 1 --branch "${HL2SDK_BRANCH}" https://github.com/alliedmodders/hl2sdk.git hl2sdk-dods
else
    echo "[INFO] Updating HL2SDK..."
    cd hl2sdk-dods
    git fetch --depth 1 origin "${HL2SDK_BRANCH}"
    git checkout "${HL2SDK_BRANCH}"
    git pull origin "${HL2SDK_BRANCH}"
    cd ..
fi

# ---- Configure ----
echo ""
echo "[INFO] Configuring build for ${TARGET}..."
cd "${ROOT}"

if [ -d "${BUILD}" ]; then
    rm -rf "${BUILD}"
fi
mkdir -p "${BUILD}"
cd "${BUILD}"

CC="${CC}" CXX="${CXX}" python3 "${ROOT}/configure.py" \
    --sm-path="${DEPS}/sourcemod" \
    --mms-path="${DEPS}/metamod-source" \
    --hl2sdk-root="${DEPS}" \
    --enable-optimize \
    --targets="${TARGET}"

# ---- Build ----
echo ""
echo "[INFO] Building..."
ambuild

# ---- Package ----
echo ""
echo "[INFO] Packaging..."
PKG_DIR="${BUILD}/dodhooks-${TARGET}"
rm -rf "${PKG_DIR}"
mkdir -p "${PKG_DIR}/addons/sourcemod/extensions"
mkdir -p "${PKG_DIR}/addons/sourcemod/gamedata"
mkdir -p "${PKG_DIR}/addons/sourcemod/scripting/include"

# Copy extension binary
find "${BUILD}/package" -name "dodhooks.ext.*" -exec cp {} "${PKG_DIR}/addons/sourcemod/extensions/" \;

# Copy gamedata
cp "${ROOT}/sourcemod/gamedata/dodhooks.txt" "${PKG_DIR}/addons/sourcemod/gamedata/"

# Copy include
cp "${ROOT}/sourcemod/scripting/include/dodhooks.inc" "${PKG_DIR}/addons/sourcemod/scripting/include/"

# Create autoload
touch "${PKG_DIR}/addons/sourcemod/extensions/dodhooks.autoload"

# Version info
cat > "${PKG_DIR}/VERSION.txt" << EOF
DoDHooks Extension
Build Date: $(date -u +%Y-%m-%d)
Architecture: ${TARGET}
Platform: Linux
SourceMod Branch: ${SM_BRANCH}
Compiler: ${CC} / ${CXX}
EOF

# Create archive
cd "${PKG_DIR}"
ZIP_NAME="${ROOT}/dodhooks-linux-${TARGET}.zip"
rm -f "${ZIP_NAME}"
zip -r "${ZIP_NAME}" .

echo ""
echo "============================================================"
echo " BUILD COMPLETE"
echo "============================================================"
echo " Output: dodhooks-linux-${TARGET}.zip"
echo "============================================================"
