#!/usr/bin/env python

import argparse
import os
import stat
import multiprocessing
from cmd_helpers import *

def parse_args(argv):
    parser = argparse.ArgumentParser()
    parser.add_argument("--output", default="none", help="Output file format", choices=["none", "hrf", "xml"])

    return parser.parse_args(argv)

def find_executables(path):
    is_exec = stat.S_IEXEC | stat.S_IXGRP | stat.S_IXOTH
    execs = []
    for entry in os.listdir(path):
        entry_path = os.path.join(path, entry)
        if os.path.isfile(entry_path):
            st = os.stat(entry_path)
            mode = st.st_mode
            if mode & is_exec:
                execs += [entry_path]
        elif os.path.isdir(entry_path):
            execs += find_executables(entry_path) 
    return execs

def write_to_file(filename, content):
    f = open(filename, "w")
    f.write(content)
    f.close()

def print_pass(msg):
    print(wrap_green("[PASS] " + msg))

def print_fail(msg):
    print(wrap_red("[FAIL] " + msg))

def main():
    options = parse_args(map(str.lower, sys.argv[1:]))

    # Fetch system info
    system, node, release, version, machine, processor = platform.uname()
    cpu_count = multiprocessing.cpu_count()

    build_dir = "build/%s.%s.%s" % (system, machine, 'Debug')
    test_path = os.path.join(build_dir, "game", "test")
    os.chdir(test_path)
    test_exec_list = find_executables(".")

    # Print found tests
    print_header("%d tests found" % (len(test_exec_list)))
    for test_exec in test_exec_list:
        print("  - " + test_exec)
    print("")

    # Options for test binary
    boost_test_options = ["--log_level=" + "all",
                          "--report_level=" + "detailed"]

    if options.output != "none":
        boost_test_options += ["--output_format=" + str.upper(options.output)]

    # TODO Support multiple jobs

    # Run tests
    print_header("Beginning test run")

    num_failed = 0
    for test_exec in test_exec_list:
        exitcode, out, err = run_cmd_capture(test_exec, boost_test_options)
        if exitcode == 0:
            print_pass(test_exec)
        else:
            print_fail(test_exec)
            num_failed += 1

        if options.output != "none":
            result_ext = { "xml": "xml", "hrf": "txt" }[options.output]
            test_path = os.path.dirname(test_exec)
            test_name, _ = os.path.splitext(os.path.basename(test_exec))

            filename_log = os.path.join(test_path, test_name + "_log." + result_ext)
            filename_sum = os.path.join(test_path, test_name + "_sum." + result_ext)
            write_to_file(filename_log, out)
            write_to_file(filename_sum, err)

    print("")
    print("======== SUMMARY ========")
    print("TEST BINARIES : %3d" % len(test_exec_list))
    print("       PASSED : %3d" % (len(test_exec_list) - num_failed))
    print("       FAILED : %3d" % num_failed)
    print("=========================")

    if num_failed > 0:
        print(wrap_red("Some test(s) were FAILED. For detail, please take a look at output files for each test."))
        sys.exit(1)

if __name__ == "__main__":
    main()

