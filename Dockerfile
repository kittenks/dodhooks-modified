# DODHooks - Linux Build Container
#
# Based on the official AlliedModders build container.
# Supports both x86 (32-bit) and x86_64 (64-bit) builds.
#
# Usage:
#   docker build -t dodhooks-builder .
#
#   # 32-bit build
#   docker run --rm -v $(pwd):/work -w /work dodhooks-builder \
#     bash -c "mkdir -p build && cd build && \
#     python3 ../configure.py --sm-path /sourcemod --mms-path /mmsource --target x86 --enable-optimize && \
#     ambuild"
#
#   # 64-bit build
#   docker run --rm -v $(pwd):/work -w /work dodhooks-builder \
#     bash -c "mkdir -p build && cd build && \
#     python3 ../configure.py --sm-path /sourcemod --mms-path /mmsource --target x86_64 --enable-optimize && \
#     ambuild"

FROM ghcr.io/alliedmodders/build-containers/debian11-clang22:latest

# Install additional dependencies
RUN apt-get update && apt-get install -y --no-install-recommends \
    python3-pip \
    git \
    && rm -rf /var/lib/apt/lists/*

# Install AMBuild
RUN pip3 install --upgrade git+https://github.com/alliedmodders/ambuild.git

# Set working directory
WORKDIR /work

# Default command: show help
CMD ["bash", "-c", "echo 'DODHooks Docker Builder' && echo '' && echo 'Usage:' && echo '  docker run --rm -v \$(pwd):/work -w /work <image> <build-command>'"]
