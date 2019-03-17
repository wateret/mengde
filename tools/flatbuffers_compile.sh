#!/usr/bin/env bash

# Generate `game/src/core/scenario_generated.h`

REPO_PATH=$(git rev-parse --show-toplevel)
SCHEMA_DIR="$REPO_PATH/game/src/core"

flatc -o $SCHEMA_DIR --cpp $SCHEMA_DIR/scenario.fbs
