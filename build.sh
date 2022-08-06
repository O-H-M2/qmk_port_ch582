#!/bin/zsh zsh

cmake --build /Users/huckies/Downloads/qmk_port_ch582/build --config Release --target all -- && \
cp ./build/qmk_port_ch582.hex ./qmk_port_ch582.hex