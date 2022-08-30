# CGP

- [Introduction](#Introduction)
- [Compile the library](#Compilation)


<a name="Introduction"></a>

## Introduction

[CGP - Computer Graphics Programming -](https://imagecomputing.net/cgp/index.html) is a lightweight and minimalist C++ library using OpenGL to represent, animate, and interact with 3D scenes. The library is primarily thought to be used for practice teaching Computer Graphics and 3D Animation. 

It provides a set of simple structures and functions (vectors, matrices, mesh structures, etc) that are simple to use and read. The objective is to save time compared to raw OpenGL coding, while remaining in the spirit and fully compatible with direct OpenGL calls.


The main spirit of CGP is to provide
* **Easy to use CG basic tools**. 
* **Data and structure ready for simple interaction and animation**. 
* **Helping structures and functions that do not impose a framework**.
* **Easy to read and understand source code**. 
* **Secured and easy to debug code**. 

[See CGP Documentation](https://imagecomputing.net/cgp/index.html)

Various examples of CGP use can be found in [scenes/examples/](scenes/examples/)

<a name="Compilation"></a>

## Compile the library

The directory _library/_ contains the source code of CGP, while the directory _examples/_ contains a set of example scenes.
Each example is an independant program with its own Makefile and/or CMakeLists.txt. 

The examples should be run from their root path where _shaders/_ (and possibly _assets/_) directories are accessible.

### Dependencies

CGP requires
* A C++14 (or greater) compatible compiler (GCC/CLang, or a recent Visual Studio).
* An OpenGL 3.3 (or greater) compatible system.
* [libGLFW](https://www.glfw.org/) and [pkgconfig](https://www.freedesktop.org/wiki/Software/pkg-config/) installed for Linux/MacOS system.

### Linux/MacOS

Assuming a command line opened in one of the example scene.
* ex. scenes/examples/01_cgp_usage/01_basic_usage/03_mesh_load_file_obj/


* _Method 1._ Using the provided Makefile:
```c++
$ make -j$(nproc)
$ ./[executable-name]
```

* _Method 2._ Using the provided CMakeLists.txt:
```c++
$ mkdir build
$ cd build
$ cmake ..
$ make
$ cd ..
$ build/[executable-name]
```

### Windows


* _Method 1._ Create a Visual Studio project using CMake
* _Method 2._ Open the CMakeLists.txt using the internal CMake tool from Visual.

_Once opened by Visual Studio, the project should be configured to compile and be executed directly without further setup. Make sure your Windows version is updated for Visual Studio to be able to compile correctly C++14._


### Detailed system set-up and compilation

A detailed tutorial on how to install and compile C++ code is available here if needed: [Detailed installation and compilation for CGP](https://imagecomputing.net/cgp/compilation).
