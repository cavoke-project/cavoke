FROM gitpod/workspace-full

RUN sudo apt-get update -yqq \
    && sudo apt-get install -yqq --no-install-recommends software-properties-common \
    openssl libssl-dev libjsoncpp-dev uuid-dev zlib1g-dev libc-ares-dev\
    postgresql-server-dev-all \
    libboost-all-dev python3-pip \
    qtbase5-dev qtdeclarative5-dev libqt5networkauth5-dev\
    libkf5archive-dev \
    && sudo rm -rf /var/lib/apt/lists/*
