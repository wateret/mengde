#!/usr/bin/env python

import argparse
import multiprocessing
import time
import os
from cmd_helpers import *

def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("buildtype",   default="Debug",
                        help="build type", choices=["Debug", "Release"], nargs="?")
    parser.add_argument("--buildpath", default="build",
                        help="specify build path")
    parser.add_argument("--clean",     default=False,
                        help="clean before build", dest="clean", action="store_true")
    parser.add_argument("--single",    default=False,
                        help="disalbe parallel build", dest="single", action="store_true")
    parser.add_argument("--cross",     default="",
                        help="cross build target", choices=["", "armv7l"])
    parser.add_argument("--generator", default="Unix Makefiles",
                        help="cmake generator", choices=["Unix Makefiles", "Xcode"])

    return parser.parse_args()

def main():
    options = parse_args()

    # Fetch system info
    system, node, release, version, machine, processor = platform.uname()
    cpu_count = multiprocessing.cpu_count()

    if options.cross == "armv7l":
        machine = "armv7l"

    # Set the install folder
    install_folder = "bin"

    home_folder = os.getcwd()

    print_header("Start CMake configuration")
    build_dir = "build/%s.%s.%s" % (system, machine, options.buildtype)
    check_run_cmd("mkdir", ["-p", build_dir])
    os.chdir(build_dir)

    cmake_config = ["-G",
                    options.generator,
                    "../" * (build_dir.count("/") + 1),
                    "-DCMAKE_BUILD_TYPE=" + options.buildtype,
                    "-DCMAKE_INSTALL_PREFIX=./",
                    "-DCMAKE_EXPORT_COMPILE_COMMANDS=1"]

    if options.cross == "armv7l":
        if os.environ.get("ROOTFS_ARM") is None:
            os.environ["ROOTFS_ARM"] = os.path.join(home_folder, "cross/rootfs/armv7l")
        toolchain = os.path.join(home_folder, "cmake/toolchain-armv7l.cmake")
        cmake_config += ["-DCMAKE_TOOLCHAIN_FILE=" + toolchain]

    check_run_cmd("cmake", cmake_config)

    if options.generator != "Unix Makefiles":
        return

    # From here, support `Makefile` project only
    make_args = []

    if options.clean:
        print_header("Clean build directory")
        check_run_cmd("make", ["clean"])

    if not options.single:
        jobs = "-j" + str(cpu_count * 2)
        make_args.append(jobs)

    print_header("Start build")
    start_time = time.time()
    check_run_cmd("make", make_args)
    elapsed_time = time.time() - start_time
    print("Build Time is %d:%02d.%02d" % (elapsed_time // 60, elapsed_time % 60, (elapsed_time * 100) % 100))

    # Temporary works for res (Font)
    res_path = os.path.join("game", "res")
    if not path_exists(res_path):
        os.mkdir(res_path)

    print(os.getcwd())

    font_url = "https://github.com/powerline/fonts/raw/master/LiberationMono/Literation%20Mono%20Powerline.ttf"
    font_filename = "Literation Mono Powerline.ttf"
    font_filepath = os.path.join(res_path, font_filename)
    if not path_exists(font_filepath):
        download_file(font_url, font_filepath)

    # Run install
    check_run_cmd("make", ["install"])

    # Copy font to install folder
    res_ipath = os.path.join(install_folder, "res")
    if not path_exists(res_ipath):
        os.mkdir(res_ipath)

    font_ifilepath = os.path.join(res_ipath, font_filename)
    if path_exists(font_filepath) and not path_exists(font_ifilepath):
        shutil.copyfile(font_filepath, font_ifilepath)

if __name__ == "__main__":
    main()

