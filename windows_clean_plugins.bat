set BUILD_DIR=.\plugins\build

if not exist %BUILD_DIR% (
    mkdir %BUILD_DIR%
)

cd %BUILD_DIR%
cmake --build . --target clean
if errorlevel 1 (
    echo Compilation failed
    exit 84
)
