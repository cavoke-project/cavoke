FROM ghcr.io/cavoke-project/cavoke-ci:server

# Build arguments. Use `--build-arg` in docker run
ARG SERVER_PORT=8080

EXPOSE $SERVER_PORT

# Cavoke server
ENV CAVOKE_ROOT="$IROOT/cavoke/"
ADD ./ $CAVOKE_ROOT
WORKDIR $CAVOKE_ROOT
# Build server
RUN chmod +x ./build.sh && ./build.sh

ENTRYPOINT $CAVOKE_ROOT/build/cavoke_server