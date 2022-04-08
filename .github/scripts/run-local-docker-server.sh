#!/usr/bin/env sh
docker run -t -p 8080:8080 cavoke_server "$@" # TODO change port by option / export port. currently just uses 8080
