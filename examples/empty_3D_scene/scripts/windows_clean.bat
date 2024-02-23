@ECHO OFF

Rem Make sure we are in the directory containing the .bat script
cd %~dp0

Rem Go to the root directory
cd ..

Rem Remove build/ if it exists
if exist build\ (
    ECHO Remove build/
    RMDIR /s /q "build"
)

for %%f in (*.pdb) do (
    ECHO Remove %%f
    DEL %%f
)

if exist imgui.ini (
    ECHO Remove imgui.ini
    DEL imgui.ini
)

ECHO.
ECHO Directory Cleaned
ECHO.

PAUSE