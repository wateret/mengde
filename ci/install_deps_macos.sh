#!/usr/bin/env bash

# Disabled below option due to python2 (2to3) link issue
# set -e

brew update > /dev/null

PACKAGES=(python2 cmake sdl2 sdl2_ttf lua boost clang-format)

for dep in "${PACKAGES[@]}"; do
  echo "Installing $dep"
  brew info "${dep}" | grep --quiet 'Not installed' && brew install "${dep}" > /dev/null
done

exit 0
