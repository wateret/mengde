## How to Build

### Required Tools and Packages

* CMake
* SDL2
* SDL2 TTF
* Lua 5.2

### macOS

```
$ brew install sdl2 sdl2_ttf lua
$ ./build.py
$ build/Darwin.x86_64/game/game
```

### Ubuntu

```
$ apt-get install libsdl2-dev libsdl2-ttf-dev liblua5.2-dev
$ ./build.py
$ build/Linux.x86_64/game/game
```

### Windows

TBD

## Source

cmake scripts below are from [here](https://github.com/tcbrindle/sdl2-cmake-scripts)

* `FindSDL2.cmake`
* `FindSDL2_image.cmake`
* `FindSDL2_ttf.cmake`

And this is from [here](https://github.com/Kitware/CMake)

* [`FindLua.cmake`](https://raw.githubusercontent.com/Kitware/CMake/master/Modules/FindLua.cmake)

