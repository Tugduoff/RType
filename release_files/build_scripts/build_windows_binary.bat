@echo off

set BUILD_DIR=.\build
set BINARY_DIR=.\bin

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

cd ../..

echo Windows build completed.
