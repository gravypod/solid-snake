# 2D Game Engine

This repository contains a 2D game engine written in C.

# License

All code is licensed under LICENSE.txt.

## Attributions

1. ./src/game/model/agumon.obj -> https://github.com/engineerOfLies/gf3d/tree/in_progress
2. ./src/game/textures/agumon.png -> https://github.com/engineerOfLies/gf3d/tree/in_progress

# How to start development...

Use `git clone --recursive <url>` to clone this repo.

## If you forgot....

```bash
git submodule update --init --recursive
```

## Pulling Changes

```bash
git pull --recursive-submodules
```

# Building

## Compilation

| Library Name | Package          | Function                   |
|--------------|------------------|----------------------------|
| dl           | N/A              | Dynamically load libraries |
| m            | N/A              | Standard math library      |
| GLFW3        | libglfw3-dev     | GLFW 3 headers             |
| Python 2     | python2          | Python 2 to build duktape  |
| Ninja        | ninja-build      | Used by google's shaderc   |

## Used Libraries

| Library Name | Function                           | URL                                    |
|--------------|------------------------------------|----------------------------------------|
| Duktape      | JavaScript runtime and C interface | http://duktape.org/                    |
| glad         | An OpenGL Loader                   | https://github.com/dav1dde/glad-web    |
| linmath      | Header based matrix/vector library | https://github.com/datenwolf/linmath.h |
| STB          | Used for image parsing functions   | https://github.com/nothings/stb        |

## Build System

I'm using CMake & GCC for this project. To build this project you must...

1. Clone the Project
2. `mkdir build # Make a new build directory`
3. `cd build # Enter directory`
4. `cmake .. # Build the Makefile from cmake`
5. `make # Build the project`
6. `./engine # Run the project`

Your CWD must be within the build directory.

# Keybindings

| Key | Function    |
|-----|-------------|
| W   | Move up     |
| A   | Move Right  |
| S   | Move Down   |
| D   | Move Right  |
| ESC | Reset Board |

It's snake. You likely know what you're supposed to do. You're the white dot. Touch the red dot. Don't touch yourself.
