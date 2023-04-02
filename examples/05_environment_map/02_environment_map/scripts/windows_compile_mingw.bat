@ECHO OFF

Rem Assume MinGW installed and configured via MSYS2 https://www.msys2.org/
Rem mingw32-make.exe must be in the windows path (you may follow this tutorial https://code.visualstudio.com/docs/languages/cpp for installation)

Rem Make sure we are in the directory containing the .bat script
cd %~dp0

Rem Go to the root directory
cd ..

Rem Created build/ if needed
if not exist build\ (
    ECHO Generate build/ directory
    ECHO mkdir build
    mkdir build
) else (
    ECHO Found existing build/ directory
)
ECHO.


ECHO Run CMake (from build/ directory)
ECHO ===============================

cd build/

cmake -G "MinGW Makefiles" .. && (
    ECHO.
    ECHO ===============================
    ECHO CMAKE Generated with success
    ECHO.
) || (
    ECHO.
    ECHO ===============================
    ECHO CMAKE Failed to generate the project
    ECHO Please check the error indicated by CMAKE
    ECHO.
)


ECHO.
ECHO Start Compiling
ECHO.
mingw32-make.exe -j8

ECHO.
ECHO If the compilation suceed, the executable file should be in the build/ directory
ECHO.

PAUSE