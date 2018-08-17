# How to Build

## Required Tools and Packages

* CMake
* SDL2
* SDL2 TTF
* Lua 5.2
* boost >= 1.52

### macOS

```
$ ci/install_deps_macos.sh
$ tools/build.py
$ tools/play.py
```

### Ubuntu

```
$ ci/install_deps_ubuntu.sh
$ tools/build.py
$ tools/play.py
```

For Raspberry Pi, run with `-r` option to change graphics resolution.
```
$ tools/play.py -r
```

#### Build with Docker

Take the following steps to build mengde through Docker:

1. Install Docker on your machine as described in the [Docker documentation](https://docs.docker.com/install/).
2. Optionally, create a Linux group called docker to allow launching containers without sudo as described in the [Docker documentation](https://docs.docker.com/install/linux/linux-postinstall/). (If you don't do this step, you'll have to use sudo each time you invoke Docker.)
3. Create a Docker image for building the mengde.
```
$ docker build -t {image_tag_name} docker
```

4. Launch a Docker container that contains one of created binary images for building.
```
$ docker run --rm -v `pwd`:{mount_path_on_container} -w={mount_path_on_container} {image_tag_name} tools/build.py
```

#### Cross build for ARM(Raspberry Pi 2/3)

You need to install required packages
```
sudo apt-get install qemu qemu-user-static binfmt-support debootstrap
sudo apt-get install binutils-arm-linux-gnueabihf
sudo apt-get install gcc-arm-linux-gnueabihf g++-arm-linux-gnueabihf
```

You then need to prepare a root filesystem. Only ARMv7l is supported for now.
```
$ sudo ./cross/build-rootfs.sh
```
Root file system will be prepared in `./cross/rootfs/armv7l`

After it's prepared, you can build with `--cross` option.
```
$ tools/build.py --cross armv7l
```

### Windows

TBD

