FROM kdeorg/ci-suse-qt515:latest
RUN zypper -n install karchive-devel
CMD ["/bin/bash"]