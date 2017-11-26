#!/usr/bin/python

import argparse
import sys
import subprocess
import os
import platform

def wrap_gen(color):
    return lambda s: color + s + "\033[0m"

wrap_red    = wrap_gen("\033[1;31m")
wrap_yellow = wrap_gen("\033[1;33m")
wrap_green  = wrap_gen("\033[1;32m")
wrap_blue   = wrap_gen("\033[1;34m")

def cmd_to_string(cmd, args = []):
    return ' '.join([cmd] + args)

def fail(msg):
    print
    print wrap_red(msg)
    print
    exit(1)

def print_cmd(cmd, args):
    print wrap_blue(cmd_to_string(cmd, args))

def run_cmd(cmd, args = []):
    print_cmd(cmd, args)
    return subprocess.call([cmd] + args)

def check_run_cmd(cmd, args = []):
    retcode = run_cmd(cmd, args)
    if retcode != 0:
        fail("[Failed %d] %s" % (retcode, cmd_to_string(cmd, args)))

def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("buildtype",   default="Debug", help="build type", choices=["Debug", "Release"], nargs="?")
    parser.add_argument("--buildpath", default="build", help="specify build path")
    parser.add_argument("--clean",     default=False,   help="clean before build", dest="clean", action="store_true")

    return parser.parse_args()

def main():
    system, node, release, version, machine, processor = platform.uname()
    build_dir = "build/%s.%s" % (system, machine)

    options = parse_args()

    check_run_cmd("mkdir", ["-p", build_dir])
    os.chdir(build_dir)
    check_run_cmd("cmake", ["-DCMAKE_BUILD_TYPE=" + options.buildtype,
                            "../../"])

    make_args = ["-j8"]
    if options.clean:
        make_args += ["clean"]

    check_run_cmd("make", make_args)

if __name__ == "__main__":
    main()

