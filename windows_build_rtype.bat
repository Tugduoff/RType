set BUILD_DIR=.\src\build

if not exist %BUILD_DIR% (
    mkdir %BUILD_DIR%
)

echo R-Type Building...
cd %BUILD_DIR%
cmake .. 
if errorlevel 1 (
    echo Compilation failed
    exit 84
)

echo R-Type Compilation...
cmake --build .
if errorlevel 1 (
    echo Compilation failed
    exit 84
)
