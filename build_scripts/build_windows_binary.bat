@REM setlocal enabledelayedexpansion

REM Define directories
set BUILD_DIR=.\build
set BINARY_DIR=.\bin

@REM REM Define SFML .dll files (Windows equivalent to .so)
@REM set SFML_DLL_FILES=(
@REM     "libsfml-audio-2.dll"
@REM     "libsfml-graphics-2.dll"
@REM     "libsfml-network-2.dll"
@REM     "libsfml-window-2.dll"
@REM     "libsfml-system-2.dll"
@REM )

REM Compile the R-Type project
echo R-Type Compilation...

if not exist %BUILD_DIR% (
    mkdir %BUILD_DIR%
)

cd %BUILD_DIR%
cmake .. 
if errorlevel 1 (
    echo Compilation failed
    exit 84
)
cmake --build .
if errorlevel 1 (
    echo Compilation failed
    exit 84
)
cd ..

REM Find all SFML .dll files and copy them to the binary directory
@REM for %%F in (%BUILD_DIR%\*.dll) do (
@REM     @REM copy %%F %BINARY_DIR%
@REM     echo "%%F"
@REM )
@REM echo All SFML .dll files have been copied to %BINARY_DIR%.

echo Windows build completed.
