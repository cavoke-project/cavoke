FROM gitpod/workspace-full

RUN sudo apt-get update -yqq \
    && sudo apt-get install -yqq --no-install-recommends software-properties-common \
    openssl libssl-dev libjsoncpp-dev uuid-dev zlib1g-dev libc-ares-dev\
    postgresql-server-dev-all \
    libboost-all-dev python3-pip \
    libkf5archive-dev \
    && sudo rm -rf /var/lib/apt/lists/* \

# INFO: Only Qt5, because installing Karchive on Qt6 is a pain...
RUN pip install aqtinstall && aqt install-qt linux desktop 5.15.2 gcc_64 -m qtnetworkauth