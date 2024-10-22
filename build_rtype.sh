#!/usr/bin/env bash

cd ./levelCreator

mkdir -p build && cd ./build

cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake --build . --target all