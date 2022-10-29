#!/bin/bash
mkdir -p build
cd build
cmake ..
cd -
cmake --build ./build --config Release --target all -- -j2
