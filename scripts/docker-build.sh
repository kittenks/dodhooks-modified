#!/bin/bash
# ============================================================
# DoDHooks - Docker Build Script
# Builds the extension for both x86 and x86_64 on Linux
# using the provided Dockerfile
# ============================================================

set -e

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
OUTPUT="${ROOT}/output"

echo "============================================================"
echo " DoDHooks Docker Build"
echo "============================================================"
echo ""

# Check Docker
if ! command -v docker >/dev/null 2>&1; then
    echo "[ERROR] Docker not found. Please install Docker first."
    echo "  https://docs.docker.com/get-docker/"
    exit 1
fi

mkdir -p "${OUTPUT}"

echo "[INFO] Building Docker image..."
docker build -f "${ROOT}/docker/Dockerfile.linux" -t dodhooks-builder "${ROOT}"

echo ""
echo "[INFO] Running build in container..."
docker run --rm -v "${OUTPUT}:/output" dodhooks-builder

echo ""
echo "============================================================"
echo " BUILD COMPLETE"
echo "============================================================"
echo " Output files in: ${OUTPUT}/"
ls -la "${OUTPUT}/"
echo "============================================================"
