#!/usr/bin/env bash

cd ./plugins

mkdir -p build && cd ./build

cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake --build . --target all
