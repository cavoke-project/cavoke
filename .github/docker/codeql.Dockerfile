FROM ubuntu:latest
RUN apt-get update && apt-get install -y clang-format clang-tidy cppcheck && rm -rf /var/lib/apt/lists/*
CMD ["/bin/bash"]