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
    exit /b 84
)

echo R-Type Compilation...
cmake --build .

if errorlevel 1 (
    echo ERROR: Compilation failed !
    exit /b 84
)

cd ../..
