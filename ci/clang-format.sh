#!/usr/bin/env bash

set -e

PROJECT_PATH=$(git rev-parse --show-toplevel)
SOURCE_PATH=$PROJECT_PATH/game
CLANG_FORMAT="clang-format"

find "$SOURCE_PATH" -type f \( -name "*.h" -o -name "*.cpp" \) -print0 | xargs -0 "$CLANG_FORMAT" -i -style=file
