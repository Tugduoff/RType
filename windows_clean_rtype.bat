@echo off

set BUILD_DIR=.\src\build

if not exist %BUILD_DIR% (
    mkdir %BUILD_DIR%
)

cd %BUILD_DIR%
cmake --build . --target clean
