#!/bin/bash
#
# DODHooks - Build Script (Linux)
#
# Builds the extension for all 4 target configurations:
#   - SourceMod 1.12 / 1.13 x Linux x86 / x86_64
#
# Usage:
#   chmod +x build.sh
#   ./build.sh
#
# Environment variables:
#   SM_BRANCH    SourceMod branch to build against (default: 1.12-dev)
#   TARGET_ARCH  Architecture to build (default: all, options: x86, x86_64)
#   ENABLE_OPT   Enable optimization (default: 1)
#   DEBUG        Enable debug build (default: 0)
#

set -e

# --- Configuration ---
SM_BRANCH="${SM_BRANCH:-1.12-dev}"
TARGET_ARCH="${TARGET_ARCH:-all}"
ENABLE_OPT="${ENABLE_OPT:-1}"
DEBUG="${DEBUG:-0}"

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_ROOT="${SCRIPT_DIR}/builds"

echo "=========================================="
echo " DODHooks Build Script"
echo "=========================================="
echo " SourceMod branch: ${SM_BRANCH}"
echo " Target arch:      ${TARGET_ARCH}"
echo " Optimize:         ${ENABLE_OPT}"
echo " Debug:            ${DEBUG}"
echo "=========================================="

# --- Clone dependencies ---
echo ""
echo "[1/3] Cloning dependencies..."

if [ ! -d "${SCRIPT_DIR}/mmsource" ]; then
    echo "  Cloning Metamod:Source..."
    git clone --depth 1 --recurse-submodules -j8 --shallow-submodules \
        -b "${SM_BRANCH}" https://github.com/alliedmodders/metamod-source.git \
        "${SCRIPT_DIR}/mmsource"
else
    echo "  Metamod:Source already exists, updating..."
    cd "${SCRIPT_DIR}/mmsource"
    git pull --ff-only || true
    cd "${SCRIPT_DIR}"
fi

if [ ! -d "${SCRIPT_DIR}/sourcemod" ]; then
    echo "  Cloning SourceMod..."
    git clone --depth 1 --recurse-submodules -j8 --shallow-submodules \
        -b "${SM_BRANCH}" https://github.com/alliedmodders/sourcemod.git \
        "${SCRIPT_DIR}/sourcemod"
else
    echo "  SourceMod already exists, updating..."
    cd "${SCRIPT_DIR}/sourcemod"
    git pull --ff-only || true
    cd "${SCRIPT_DIR}"
fi

# --- Build function ---
build_target() {
    local arch="$1"
    local sm_label="$2"
    local build_dir="${SCRIPT_DIR}/build_${sm_label}_${arch}"

    echo ""
    echo "---------------------------------------------------"
    echo " Building for ${sm_label} / ${arch}"
    echo "---------------------------------------------------"

    mkdir -p "${build_dir}"
    cd "${build_dir}"

    local configure_args=""
    configure_args+=" --sm-path ${SCRIPT_DIR}/sourcemod"
    configure_args+=" --mms-path ${SCRIPT_DIR}/mmsource"
    configure_args+=" --target ${arch}"

    if [ "${ENABLE_OPT}" = "1" ]; then
        configure_args+=" --enable-optimize"
    fi

    if [ "${DEBUG}" = "1" ]; then
        configure_args+=" --enable-debug"
    fi

    echo "  Configuring..."
    python3 "${SCRIPT_DIR}/configure.py" ${configure_args}

    echo "  Building..."
    ambuild

    # Copy output to organized directory
    local output_dir="${BUILD_ROOT}/${sm_label}/${arch}"
    mkdir -p "${output_dir}"

    if [ "${arch}" = "x86_64" ]; then
        cp -av "package/addons/sourcemod/extensions/x64/." "${output_dir}/" 2>/dev/null || true
    else
        cp -av "package/addons/sourcemod/extensions/." "${output_dir}/" 2>/dev/null || true
    fi
    cp -av "package/addons/sourcemod/gamedata/." "${output_dir}/../gamedata/" 2>/dev/null || true

    echo "  Output: ${output_dir}"
    cd "${SCRIPT_DIR}"
}

# --- Build all targets ---
echo ""
echo "[2/3] Building extension..."

# Determine which SM branches to build
SM_BRANCHES=("1.12-dev" "master")
SM_LABELS=("1.12" "1.13")

for i in "${!SM_BRANCHES[@]}"; do
    branch="${SM_BRANCHES[$i]}"
    label="${SM_LABELS[$i]}"

    # Switch sourcemod to correct branch
    cd "${SCRIPT_DIR}/sourcemod"
    git fetch --depth 1 origin "${branch}" 2>/dev/null || true
    git checkout "${branch}" 2>/dev/null || true
    cd "${SCRIPT_DIR}"

    # Build architectures
    if [ "${TARGET_ARCH}" = "all" ] || [ "${TARGET_ARCH}" = "x86" ]; then
        build_target "x86" "${label}"
    fi

    if [ "${TARGET_ARCH}" = "all" ] || [ "${TARGET_ARCH}" = "x86_64" ]; then
        build_target "x86_64" "${label}"
    fi
done

# --- Package ---
echo ""
echo "[3/3] Creating release packages..."

cd "${SCRIPT_DIR}"

for label in "1.12" "1.13"; do
    for os_name in "linux"; do
        pkg_dir="package-${label}-${os_name}"
        mkdir -p "${pkg_dir}/addons"

        # Copy extensions
        if [ -d "${BUILD_ROOT}/${label}" ]; then
            for arch_dir in "${BUILD_ROOT}/${label}"/*/; do
                cp -a "${arch_dir}." "${pkg_dir}/addons/" 2>/dev/null || true
            done
        fi

        # Copy gamedata
        mkdir -p "${pkg_dir}/addons/sourcemod/gamedata"
        cp -a "${SCRIPT_DIR}/sourcemod/gamedata/." "${pkg_dir}/addons/sourcemod/gamedata/" 2>/dev/null || true

        # Create archive
        version=$(grep -oP 'SMEXT_CONF_VERSION\s+"\K[^"]+' "${SCRIPT_DIR}/smsdk_config.h")
        build_num=$(git rev-list --count HEAD 2>/dev/null || echo "0")
        full_version="${version}.${build_num}"

        archive="DODHooks-${full_version}-sm${label}-${os_name}.tar.gz"
        tar -czf "${archive}" -C "${pkg_dir}" addons
        echo "  Created: ${archive}"
    done
done

echo ""
echo "=========================================="
echo " Build complete!"
echo "=========================================="
echo " Output archives:"
ls -la DODHooks-*.tar.gz 2>/dev/null || true
echo ""
echo " Build artifacts: ${BUILD_ROOT}/"
ls -R "${BUILD_ROOT}/" 2>/dev/null || true
