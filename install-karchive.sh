#!/usr/bin/env sh
mkdir build-zlib
cmake -B build-zlib ./third_party/zlib
cmake --build build-zlib --target install
mkdir build-ECM
cmake -B build-ECM ./third_party/extra-cmake-modules
cmake --build build-ECM --target install
mkdir build-karchive
cmake -B build-karchive ./third_party/karchive
cmake --build build-karchive --target install