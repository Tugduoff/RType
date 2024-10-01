set BUILD_DIR=.\src\build

if not exist %BUILD_DIR% (
    mkdir %BUILD_DIR%
)

cd %BUILD_DIR%
cmake .. 
if errorlevel 1 (
    echo Compilation failed
    exit 84
)
cmake --build . --target all
if errorlevel 1 (
    echo Compilation failed
    exit 84
)
