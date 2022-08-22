#!/bin/zsh zsh

cmake --build ./build --config Release --target all -- -j2 && \
cp ./build/qmk_port_ch582.hex ./qmk_port_ch582.hex