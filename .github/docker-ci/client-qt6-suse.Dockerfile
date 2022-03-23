FROM kdeorg/ci-suse-qt62:latest

RUN pushd $HOME && \
    zypper -n install extra-cmake-modules && \
    git clone --branch v5.90.0 https://github.com/KDE/karchive && \
    mkdir karchive/build && \
    cd karchive && \
    cd build && \
    cmake .. -DCMAKE_BUILD_TYPE=debug -DCMAKE_INSTALL_PREFIX=/usr/local -DQT_MAJOR_VERSION=6 && \
    make && \
    make install && \
    popd

CMD ["/bin/bash"]
