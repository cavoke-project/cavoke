FROM drogonframework/drogon:latest

# Build arguments. Use `--build-arg` in docker run
ARG SERVER_PORT=8080

EXPOSE $SERVER_PORT
ENV DEBIAN_FRONTEND noninteractive

# Nlohmann
ENV NLOHMANN_ROOT="$IROOT/json/"
RUN git clone https://github.com/nlohmann/json $NLOHMANN_ROOT
WORKDIR $NLOHMANN_ROOT
RUN cmake . -DJSON_BuildTests=OFF && make install

# Cavoke
ENV CAVOKE_ROOT="$IROOT/cavoke/"
ADD ./* $CAVOKE_ROOT
WORKDIR $CAVOKE_ROOT
RUN chmod +x ./build.sh && ./build.sh

ENTRYPOINT ["$CAVOKE_ROOT/build/cavoke_server", "-p", "$SERVER_PORT"]