#!/usr/bin/env bash
set -eo pipefail

# Create mount directory for service
mkdir -p "$MNT_DIR"

echo "Mounting GCS Fuse."
gcsfuse --debug_gcs --debug_fuse --file-mode=777 "$BUCKET" "$MNT_DIR"
echo "Mounting completed."

# Run the web service on container startup.
exec /usr/local/bin/cavoke_server -p "$PORT" -c "$DROGON_CONFIG_FILE" "$@" &

# Exit immediately when one of the background processes terminate.
wait -n