# mengde

[![Build Status](https://travis-ci.org/wateret/mengde.svg?branch=master)](https://travis-ci.org/wateret/mengde)

Project mengde is named after the courtesy name(Mèngdé 孟德) of Cáo Cāo 曹操 from Chinese Three Kingdoms period.
This game is inspired by "三國志曹操傳" by 1998, KOEI.

There are features in common between mengde and 三國志曹操傳.

- Similar game system
    - Battle system : Turn-based with square tiles
    - Stat system : 5 stats along with HP and MP which are the same
	- Hero system : All heroes have different class and stat which makes each hero special
- Same graphical resources

## For gamers

This game is not complete for playing yet.

## For scenario(MOD) developers

TBD

## For game engine developers

For now we only have instructions for build.

### How to Build

#### Required Tools and Packages

* CMake
* SDL2
* SDL2 TTF
* Lua 5.2
* boost >= 1.52

#### macOS

```
$ brew install cmake sdl2 sdl2_ttf lua boost
$ ./build.py
$ build/Darwin.x86_64.Debug/game/game
```

#### Ubuntu

```
$ apt-get install cmake libsdl2-dev libsdl2-ttf-dev liblua5.2-dev libboost-system-dev libboost-filesystem-dev
$ ./build.py
$ build/Linux.x86_64.Debug/game/game
```

##### Building with Docker

Take the following steps to build mengde through Docker:

1. Install Docker on your machine as described in the [Docker documentation](https://docs.docker.com/install/).
2. Optionally, create a Linux group called docker to allow launching containers without sudo as described in the [Docker documentation](https://docs.docker.com/install/linux/linux-postinstall/). (If you don't do this step, you'll have to use sudo each time you invoke Docker.)
3. Create a Docker image for building the mengde.
```
$ docker build -t {image_tag_name} docker
```

4. Launch a Docker container that contains one of created binary images for building.
```
$ docker run --rm -v `pwd`:{mount_path_on_container} -w={mount_path_on_container} {image_tag_name} ./build.py
```

#### Windows

TBD

