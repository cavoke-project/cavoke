FROM ghcr.io/cavoke-project/cavoke-server

# Install system dependencies
RUN set -e; \
    apt-get update -y && apt-get install -y \
    tini \
    lsb-release \
    gpg-agent; \
    gcsFuseRepo=gcsfuse-`lsb_release -c -s`; \
    echo "deb http://packages.cloud.google.com/apt $gcsFuseRepo main" | \
    tee /etc/apt/sources.list.d/gcsfuse.list; \
    curl https://packages.cloud.google.com/apt/doc/apt-key.gpg | \
    apt-key add -; \
    apt-get update; \
    apt-get install -y gcsfuse \
    && apt-get clean

# Drogon configuration basename for
ARG DROGON_CONFIG_BASENAME=drogon_config.json

# Set fallback mount directory
ENV MNT_DIR /mnt/gcs
ENV DROGON_CONFIG_FILE $MNT_DIR/$DROGON_CONFIG_BASENAME

# Copy local code to the container image.
ENV APP_HOME /app
WORKDIR $APP_HOME
COPY gcsfuse_run.sh ./

# Ensure the script is executable
RUN chmod +x /app/gcsfuse_run.sh

# Use tini to manage zombie processes and signal forwarding
# https://github.com/krallin/tini
ENTRYPOINT ["/usr/bin/tini", "--"]

# Pass the startup script as arguments to Tini
CMD ["/app/gcsfuse_run.sh"]