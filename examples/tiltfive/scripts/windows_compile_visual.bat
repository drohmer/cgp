@ECHO OFF

Rem Assume Visual Studio is installed

Rem Make sure we are in the directory containing the .bat script
cd %~dp0

Rem Go to the root directory
cd ..

ECHO Run CMake (from build/ directory)
ECHO ===============================


cmake -B build/ && (
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
cmake --build build/ --config RelWithDebInfo


PAUSE