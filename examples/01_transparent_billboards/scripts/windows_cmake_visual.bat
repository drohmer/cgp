@ECHO OFF

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

cmake .. && (
    ECHO.
    ECHO ===============================
    ECHO CMAKE Generated with success
    ECHO The project can be loaded from the .sln file in build/ directory
    ECHO.
) || (
    ECHO.
    ECHO ===============================
    ECHO CMAKE Failed to generate the project
    ECHO Please check the error indicated by CMAKE
    ECHO.
)
Rem The previous command call cmake with the default target project
Rem   Specific targets use -G option from cmake
Rem   ex. cmake -G "Visual Studio 17 2020" ..



PAUSE