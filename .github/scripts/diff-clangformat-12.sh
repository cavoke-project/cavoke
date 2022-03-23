#!/usr/bin/env bash
diff -u <(cat "$1") <(clang-format-12 "$1") && exit 0 || exit 1