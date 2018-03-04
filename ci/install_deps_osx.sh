#!/usr/bin/env bash

brew update

PACKAGES=(python2 cmake sdl2 sdl2_ttf lua boost)

for dep in "${PACKAGES[@]}"; do
  brew info "${dep}" | grep --quiet 'Not installed' && brew install "${dep}"
done

exit 0
