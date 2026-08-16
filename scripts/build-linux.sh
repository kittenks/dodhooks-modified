#!/bin/bash
# ============================================================
# DoDHooks Linux Build Script
# Usage:  ./scripts/build-linux.sh [x86|x86_64|all]
# ============================================================

set -e

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

print_status() { echo -e "${BLUE}[*]${NC} $1"; }
print_ok()     { echo -e "${GREEN}[✓]${NC} $1"; }
print_warn()   { echo -e "${YELLOW}[!]${NC} $1"; }
print_error()  { echo -e "${RED}[✗]${NC} $1"; }

# ============================================================
# Configuration
# ============================================================
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
BUILD_TYPE="release"
TARGETS="${1:-x86_64}"

# Dependency paths (adjust these to your environment)
SM_ROOT="${SM_ROOT:-$HOME/sourcemod}"
MM_ROOT="${MM_ROOT:-$HOME/metamod-source}"
HL2SDK_ROOT="${HL2SDK_ROOT:-$HOME}"
HL2SDK_DODS="${HL2SDK_ROOT}/hl2sdk-dods"

print_status "DoDHooks Linux Build Script"
print_status "Project root: $PROJECT_ROOT"
print_status "Targets: $TARGETS"
echo ""

# ============================================================
# Check dependencies
# ============================================================
print_status "Checking dependencies..."

if ! command -v python3 &>/dev/null; then
    print_error "python3 not found. Install with: sudo apt-get install python3"
    exit 1
fi
print_ok "python3 found"

if ! python3 -c "import ambuild2" 2>/dev/null; then
    print_warn "AMBuild not found. Installing..."
    pip3 install "AMBuild==2.2.0"
fi
print_ok "AMBuild found"

if [ ! -d "$SM_ROOT" ]; then
    print_error "SourceMod not found at: $SM_ROOT"
    print_error "Set SM_ROOT environment variable or clone to $HOME/sourcemod"
    exit 1
fi
print_ok "SourceMod found: $SM_ROOT"

if [ ! -d "$MM_ROOT" ]; then
    print_error "Metamod:Source not found at: $MM_ROOT"
    print_error "Set MM_ROOT environment variable or clone to $HOME/metamod-source"
    exit 1
fi
print_ok "Metamod:Source found: $MM_ROOT"

if [ ! -d "$HL2SDK_DODS" ]; then
    print_error "HL2SDK DoD:S not found at: $HL2SDK_DODS"
    print_error "Clone with: git clone -b dods https://github.com/alliedmodders/hl2sdk hl2sdk-dods"
    exit 1
fi
print_ok "HL2SDK DoD:S found: $HL2SDK_DODS"

# ============================================================
# Install 32-bit libraries if needed
# ============================================================
if [[ "$TARGETS" == *"x86"* ]] && ! dpkg --print-foreign-architectures 2>/dev/null | grep -q i386; then
    print_status "Installing 32-bit support libraries..."
    sudo dpkg --add-architecture i386
    sudo apt-get update
    sudo apt-get install -y --no-install-recommends \
        gcc-multilib g++-multilib lib32stdc++6 \
        zlib1g-dev zlib1g-dev:i386 libc6-dev-i386 linux-libc-dev:i386
    print_ok "32-bit libraries installed"
fi

# ============================================================
# Build function
# ============================================================
build_target() {
    local arch="$1"
    print_status "Building for $arch..."

    cd "$PROJECT_ROOT"

    # Clean previous build
    rm -rf "build_$arch"
    mkdir -p "build_$arch"
    cd "build_$arch"

    # Configure
    print_status "Configuring ($arch)..."
    python3 ../configure.py \
        --targets="$arch" \
        --enable-optimize \
        --sm-path="$SM_ROOT" \
        --mms-path="$MM_ROOT" \
        --hl2sdk-root="$HL2SDK_ROOT" \
        --sdks=dod

    if [ $? -ne 0 ]; then
        print_error "Configure failed for $arch"
        return 1
    fi
    print_ok "Configure successful ($arch)"

    # Build
    print_status "Compiling ($arch)..."
    ambuild

    if [ $? -ne 0 ]; then
        print_error "Build failed for $arch"
        return 1
    fi
    print_ok "Build successful ($arch)"

    # Show output
    if [ -f "package/dodhooks.ext.so" ] || [ -f "package/dodhooks.ext.dll" ]; then
        print_ok "Artifact created:"
        ls -la package/
    fi

    cd "$PROJECT_ROOT"
    return 0
}

# ============================================================
# Execute builds
# ============================================================
FAILED=0

case "$TARGETS" in
    x86)
        build_target "x86" || FAILED=1
        ;;
    x86_64)
        build_target "x86_64" || FAILED=1
        ;;
    all)
        build_target "x86" || FAILED=1
        build_target "x86_64" || FAILED=1
        ;;
    *)
        print_error "Unknown target: $TARGETS"
        print_error "Usage: $0 [x86|x86_64|all]"
        exit 1
        ;;
esac

# ============================================================
# Summary
# ============================================================
echo ""
if [ $FAILED -eq 0 ]; then
    print_ok "All builds completed successfully!"
    echo ""
    print_status "Package contents:"
    for d in build_*/package; do
        if [ -d "$d" ]; then
            echo "  $d/:"
            ls -la "$d/" | awk '{print "    "$NF}'
        fi
    done
else
    print_error "Some builds failed. Check the logs above."
    exit 1
fi
