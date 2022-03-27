#!/usr/bin/env bash
set -eo pipefail

# Create mount directory for service
mkdir -p "$MNT_DIR"

echo "Mounting GCS Fuse."
gcsfuse --debug_gcs --debug_fuse --file-mode=005 "$BUCKET" "$MNT_DIR"
echo "Mounting completed."

# Run the web service on container startup. Here we use the gunicorn
# webserver, with one worker process and 8 threads.
# For environments with multiple CPU cores, increase the number of workers
# to be equal to the cores available.
# Timeout is set to 0 to disable the timeouts of the workers to allow Cloud Run to handle instance scaling.
exec /usr/local/bin/cavoke_server -p "$PORT" -g "$MNT_DIR" "$@" &

# Exit immediately when one of the background processes terminate.
wait -n