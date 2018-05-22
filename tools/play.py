#!/usr/bin/env python

import os
import sys
import time
import subprocess
import platform
import argparse

def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("buildtype", default="Debug", help="build type",
                        choices=["Debug", "Release"], nargs="?")
    parser.add_argument("-r", "--rpi", help="Set resolution for Raspberry Pi",
                        action="store_true")
    return parser.parse_args()

def main():
    options = parse_args()

    # Fetch system info
    system, node, release, version, machine, processor = platform.uname()

    build_dir = os.path.join("build", "%s.%s.%s" % (system, machine, options.buildtype))
    os.chdir(build_dir)

    install_folder = "mengde"
    os.chdir(install_folder)

    if (options.rpi):
        subprocess.call(['tvservice', '-e', 'DMT 16']) # change to 1024x768
        time.sleep(1) # need a little bit of time for graphics to be stable

    game_path = os.path.join(".", "game")
    subprocess.call([game_path])
    
    if (options.rpi):
        subprocess.call(["tvservice", "-p"]) # change to prefered(default) mode
        time.sleep(1)
        subprocess.call(["fbset", "-depth", "24"]) # hack to make screen visible
        subprocess.call(["fbset", "-depth", "32"])

if __name__ == "__main__":
    main()
