#!/usr/bin/env sh
pushd $HOME
# Install ECM
zypper -n install extra-cmake-modules
# Prepare KArchive
git clone --branch v5.90.0 https://github.com/KDE/karchive  # FIXME: don't use specific version: currently ECM conflicts
mkdir karchive/build
cd karchive
# Build 
cd build
cmake .. -DCMAKE_BUILD_TYPE=debug -DCMAKE_INSTALL_PREFIX=/usr/local -DQT_MAJOR_VERSION=6
make
make install
popd
