import os
import sys
import subprocess
import platform
import urllib2
import shutil

def wrap_gen(color):
    return lambda s: color + s + "\033[0m"

wrap_red    = wrap_gen("\033[1;31m")
wrap_yellow = wrap_gen("\033[1;33m")
wrap_green  = wrap_gen("\033[1;32m")
wrap_blue   = wrap_gen("\033[1;34m")

def cmd_to_string(cmd, args = []):
    return ' '.join([cmd] + args)

def fail(msg):
    print("")
    print(wrap_red(msg))
    print("")
    exit(1)

def print_cmd(cmd, args):
    print(wrap_blue(cmd_to_string(cmd, args)))

def print_header(msg):
    print(wrap_yellow(msg))

def run_cmd(cmd, args = []):
    print_cmd(cmd, args)
    return subprocess.call([cmd] + args)

def check_run_cmd(cmd, args = []):
    retcode = run_cmd(cmd, args)
    if retcode != 0:
        fail("[Failed %d] %s" % (retcode, cmd_to_string(cmd, args)))

def run_cmd_capture(cmd, args = []):
    print_cmd(cmd, args)
    pipes = subprocess.Popen([cmd] + args, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stdout, stderr = pipes.communicate()
    return pipes.returncode, stdout, stderr

def download_file(url, filename):
    try:
        f = urllib2.urlopen(url)
        print_header("Downloading " + url)

        # Open our local file for writing
        with open(filename, "wb") as local:
            local.write(f.read())
    except urllib2.HTTPError, e:
        print "HTTP Error:", e.code, url
    except urllib2.URLError, e:
        print "URL Error:", e.reason, url

def path_exists(path):
    try:
        os.stat(path)
        return True
    except:
        return False

