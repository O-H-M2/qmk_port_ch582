#!/bin/zsh zsh

cmake --build ./build --config Release --target all -- -j 4 && \
cp ./build/mk01.hex ./mk01_upgrade.hex && \
cp ./build/mk01_IAP.hex ./mk01_IAP.hex && \
mergehex --merge ./mk01_upgrade.hex ./mk01_IAP.hex --output mk01_factory.hex && \
rm ./mk01_IAP.hex