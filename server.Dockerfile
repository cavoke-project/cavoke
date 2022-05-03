FROM ghcr.io/cavoke-project/cavoke-ci:server

# Build arguments. Use `--build-arg` in docker run
ARG SERVER_PORT=8080

EXPOSE $SERVER_PORT

# Cavoke server
ENV CAVOKE_ROOT="$IROOT/cavoke/"
ADD server/ $CAVOKE_ROOT
# add cavoke-dev-lib dependency
ADD cavoke-dev-lib/ $CAVOKE_ROOT
WORKDIR $CAVOKE_ROOT
# Install server
RUN chmod +x ./install.sh && ./install.sh -DCAVOKE_H_DIR=. -DUSE_EXTERNAL_DROGON=ON -DUSE_EXTERNAL_NLOHMANN=ON
WORKDIR /
RUN rm -rf $CAVOKE_ROOT

ENTRYPOINT ["/usr/local/bin/cavoke_server"]
