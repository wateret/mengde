#!/usr/bin/env bash

set -e

sudo apt-get -qq update
sudo apt-get install -y cmake libsdl2-dev libsdl2-ttf-dev liblua5.2-dev libboost-all-dev

exit 0
