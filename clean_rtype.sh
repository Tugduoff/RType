#!/usr/bin/env bash

cd ./src

mkdir -p build && cd ./build

cmake --build . --target clean
