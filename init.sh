#!/bin/bash

# Setup python (see https://github.com/indygreg/python-build-standalone/releases/tag/20241016 for available versions)
PYTHON_VERSION="3.13.0"
ARCH="$(arch)"
if [ "$ARCH" == "" ]; then
    ARCH="$(uname -m)"
fi
PYTHON_URL="https://github.com/indygreg/python-build-standalone/releases/download/20241016/cpython-$PYTHON_VERSION+20241016-$ARCH-unknown-linux-gnu-install_only_stripped.tar.gz"
echo "Downloading Python from '$PYTHON_URL' ..."
curl -L -o python.tar.gz $PYTHON_URL
mkdir tmp
mkdir -p ./bin/
rm -rf ./bin/python3
echo "Extracting Python (IGNORE ERRORS, IT'll STILL WORK :D)..."
tar -xzf python.tar.gz -C ./tmp/
mv ./tmp/python/bin/ ./bin/python3
# Cleanup
rm -rf tmp python.tar.gz

# Setup rust (MUST HAVE RUST INSTALLED THORUGH RUSTUP ALREADY)
cp -r $HOME/.rustup/toolchains/stable-*-linux-gnu/bin bin/rust

if [ ! -d "vcpkg" ]; then
    git clone https://github.com/microsoft/vcpkg.git --depth 1
    bash vcpkg/bootstrap-vcpkg.sh -disableMetrics
fi
vcpkg/vcpkg install crow spdlog nlohmann-json
cmake -DCMAKE_TOOLCHAIN_FILE=vcpkg/scripts/buildsystems/vcpkg.cmake -S . -B build