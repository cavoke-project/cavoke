#!/usr/bin/env sh
mkdir -p build
# shellcheck disable=SC2164
cd build
cmake .. "$@" && make install