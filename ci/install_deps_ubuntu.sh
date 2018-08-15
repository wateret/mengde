#!/usr/bin/env bash

set -e

sudo apt-get -qq update

version=`lsb_release -rs`
version_int=${version:0:2}

if [ $version_int -ge 16 ]
then
sudo apt-get install -y cmake libsdl2-dev libsdl2-ttf-dev liblua5.2-dev libboost-all-dev clang-format
else
sudo apt-get install -y cmake libsdl2-dev libsdl2-ttf-dev liblua5.2-dev boost1.55 clang-format-3.4
fi

exit 0
