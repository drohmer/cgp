# CGP - Computer Graphics Programming Library

![BuildWindows](https://github.com/drohmer/CGP/actions/workflows/Windows.yml/badge.svg)
![BuildLinux](https://github.com/drohmer/CGP/actions/workflows/Linux.yml/badge.svg)
![BuildMacOS](https://github.com/drohmer/CGP/actions/workflows/MacOS.yml/badge.svg)

- [Introduction](#Introduction)
- [Compile the library](#Compilation)


<a name="Introduction"></a>

## Introduction

[CGP - Computer Graphics Programming -](https://imagecomputing.net/cgp/index.html) is a lightweight and minimalist C++ library using OpenGL to represent, animate, and interact with 3D scenes. 

It features a set of simple structures and functions (vectors, matrices, mesh structures, transforms, camera, etc) that are simple to use and read. The objective is to save time compared to raw OpenGL coding, while preserving the fundamental logic and comprehension of high-performance Graphics. The 3D scene can be exported as a Webpage via Emscripten scripts.

The main objective of CGP is to provide
* **Easy to use CG basic tools**. 
* **Data and structure ready for simple interaction and animation**. 
* **Helping structures and functions that do not impose a framework**.
* **Easy to read and understand source code**. 
* **Secured and easy to debug code**. 

### Additional documentation

* **[Library Webpage](https://imagecomputing.net/cgp/index.html)**

* **[Code Examples](https://imagecomputing.net/cgp/documentation/07_examples/index.html)**, [github-page](https://github.com/drohmer/cgp_examples)


<a name="Compilation"></a>

## Compile the library

The directory _library/_ contains the source code of CGP, while the directory _examples/_ contains a set of example scenes.
Each example is an independent program with its own CMakeLists.txt and/or Makefile. 

CGP requires
* A C++14 (or greater) compatible compiler (GCC/CLang, or a recent Visual Studio).
* An OpenGL 3.3 (or greater) compatible system.
* [libGLFW](https://www.glfw.org/) and [pkgconfig](https://www.freedesktop.org/wiki/Software/pkg-config/) installed on Linux system.


### Linux

System setup

```c++
sudo apt install build-essential pkg-config cmake
sudo apt install libglfw3-dev
```

Building examples/example_compilation/ (via CMake)
```c++
cd examples/example_compilation/
cmake -B build/
cd build/
make -j$(nproc)
./example_compilation
```

_Alternative_: Building examples/example_compilation/ (via Makefile)
```c++
cd examples/example_compilation/
make -j$(nproc)
./example_compilation
```


### Windows

**Method 1**. Create a Visual Studio project using CMake
* You may use the following batch script: examples/example_compilation/scripts/__windows_cmake_visual.bat__

_Once opened by Visual Studio, the project should be configured to compile and be executed directly without further setup. Make sure your Windows version is updated for Visual Studio to be able to compile correctly C++14._

**Method 2**. Install MinGW from [MSYS2](https://www.msys2.org/). 
* You may use the following batch script: examples/example_compilation/scripts/__windows_compile_mingw.bat__
* You may build/edit the project using VS Code.



### MacOS

Building examples/example_compilation/ (via CMake)
```c++
cd examples/example_compilation/
cmake -B build/
cd build/
make -j$(nproc)
./example_compilation
```




### Detailed system set-up and compilation

A detailed tutorial on how to install and compile C++ code is available here if needed: [Detailed installation and compilation for CGP](https://imagecomputing.net/cgp/compilation).



## Export result as a Website

CGP is set to be compatible with [Emscripten](https://emscripten.org/) compilation. A website template and compilation script is provided for Linux system (emscripten must be installed).

Generating a website for examples/example_compilation/
```c++
$ cd examples/example_compilation/
$ python scripts/linux_compile_emscripten.py
$ emrun index.html
```