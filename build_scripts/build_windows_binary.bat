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
    goto failed
)
cmake --build .
if errorlevel 1 (
    echo Compilation failed
    goto failed
)

:failed
cd ../..

echo Windows build completed.
