#!/usr/bin/env sh
cd $HOME
# Install ECM
zypper -n install extra-cmake-modules
# Prepare KArchive
git clone https://github.com/KDE/karchive
mkdir karchive/build
cd karchive
git switch -c 5.90.0  # FIXME
# Build 
cd build
cmake .. -DCMAKE_BUILD_TYPE=debug -DCMAKE_INSTALL_PREFIX=/usr/local -DQT_MAJOR_VERSION=6
make
make install
cd $CIRCLE_WORKING_DIRECTORY
