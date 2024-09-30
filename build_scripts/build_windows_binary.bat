@REM setlocal enabledelayedexpansion

REM Define directories
set BUILD_DIR=.\build
set BINARY_DIR=.\bin

REM Define SFML .dll files (Windows equivalent to .so)
set SFML_DLL_FILES=(
    "libsfml-audio-2.dll"
    "libsfml-graphics-2.dll"
    "libsfml-network-2.dll"
    "libsfml-window-2.dll"
    "libsfml-system-2.dll"
)

REM Compile the R-Type project
echo R-Type Compilation...

if not exist %BUILD_DIR% (
    mkdir %BUILD_DIR%
)

cd %BUILD_DIR%
cmake .. 
if errorlevel 1 (
    echo Compilation failed
    exit /b 84
)
cmake --build .
if errorlevel 1 (
    echo Compilation failed
    exit /b 84
)
cd ..

REM Find all SFML .dll files and copy them to the binary directory
for %%F in (%BUILD_DIR%\*.dll) do (
    @REM copy %%F %BINARY_DIR%
    echo %%F
)
@REM echo All SFML .dll files have been copied to %BINARY_DIR%.

echo Windows build completed.
