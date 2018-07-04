#!/usr/bin/env bash

set -e

sudo apt-get -qq update
sudo apt-get install -y cmake libsdl2-dev libsdl2-ttf-dev liblua5.2-dev libboost-all-dev clang-format-6.0
sudo rm /usr/bin/clang-format -f
sudo ln -s /usr/bin/clang-format-6.0 /usr/bin/clang-format

exit 0
