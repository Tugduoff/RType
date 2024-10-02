#!/usr/bin/env bash

cd ./plugins

mkdir -p build && cd ./build

cmake --build . --target clean
