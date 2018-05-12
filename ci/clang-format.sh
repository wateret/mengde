#!/usr/bin/env bash

set -e

PROJECT_PATH=$(git rev-parse --show-toplevel)
SOURCE_PATH=$PROJECT_PATH/game
CLANG_FORMAT="clang-format"

echo "[clang-format] Format check begins."

find "$SOURCE_PATH" -type f \( -name "*.h" -o -name "*.cc" \) -print0 | xargs -0 "$CLANG_FORMAT" -i -style=file

set +e

git diff-index --quiet HEAD --
STATUS=$?

if [ "$STATUS" -eq "0" ]; then
   echo "[clang-format] Format check succeeded."
else
   echo "[clang-format] Format check failed. Please run 'git diff' to check out the changes."
fi
exit $STATUS
