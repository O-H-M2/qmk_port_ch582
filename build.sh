#!/bin/zsh zsh

cmake -G"Eclipse CDT4 - Unix Makefiles"  -DCMAKE_BUILD_TYPE=Release && \
make -j8