FROM ubuntu:latest
RUN apt-get update && apt-get install -y clang-format-12 clang-tidy-12 cppcheck && rm -rf /var/lib/apt/lists/*
CMD ["/bin/bash"]