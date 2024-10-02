@echo off

set BUILD_DIR=.\plugins\build

if not exist %BUILD_DIR% (
    mkdir %BUILD_DIR%
)

echo R-Type Building...
cd %BUILD_DIR%
cmake .. 

if errorlevel 1 (
    echo ERROR: Build failed !
    goto failed
)

echo R-Type Compilation...
cmake --build .

if errorlevel 1 (
    echo ERROR: Compilation failed !
    goto failed
)

:failed
cd ../..
