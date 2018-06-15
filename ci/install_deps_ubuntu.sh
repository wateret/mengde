#!/usr/bin/env bash

set -e

sudo apt-get -qq update

if [[ `lsb_release -rs` -lt 16 ]]
then
sudo apt-get install -y cmake libsdl2-dev libsdl2-ttf-dev liblua5.2-dev libboost-all-dev clang-format
else
sudo apt-get install -y cmake libsdl2-dev libsdl2-ttf-dev liblua5.2-dev boost1.55 clang-format-3.4
fi

exit 0
