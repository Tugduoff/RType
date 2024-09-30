#!/usr/bin/env bash

cd ./plugins

mkdir -p build && cd ./build

cmake ..
cmake --build . --target clean
cmake --build . --target all

