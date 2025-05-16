#!/bin/bash

mkdir build
cd build

cmake .. --fresh \
    -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
    -DCMAKE_BUILD_TYPE=Release

cmake --build .