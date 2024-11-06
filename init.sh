#!/bin/bash

if [ ! -d "vcpkg" ]; then
    git clone https://github.com/microsoft/vcpkg.git
    bash vcpkg/bootstrap-vcpkg.sh -disableMetrics
fi
vcpkg/vcpkg install crow spdlog nlohmann-json
cmake -DCMAKE_TOOLCHAIN_FILE=vcpkg/scripts/buildsystems/vcpkg.cmake -S . -B build