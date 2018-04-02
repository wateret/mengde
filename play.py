#!/usr/bin/env python

import os
import sys
import subprocess
import platform
import argparse

def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("buildtype", default="Debug", help="build type",
                        choices=["Debug", "Release"], nargs="?")
    return parser.parse_args()

def main():
    options = parse_args()

    # Fetch system info
    system, node, release, version, machine, processor = platform.uname()

    build_dir = os.path.join("build", "%s.%s.%s" % (system, machine, options.buildtype))
    os.chdir(build_dir)

    install_folder = "mengde"
    os.chdir(install_folder)

    game_path = os.path.join(".", "game")
    subprocess.call([game_path])

if __name__ == "__main__":
    main()
