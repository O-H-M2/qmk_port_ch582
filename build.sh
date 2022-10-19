#!/bin/zsh zsh

keyboard=mk02

cmake --build ./build --config Release --target all -- -j 4 && \
cp ./build/${keyboard}.hex ./${keyboard}_upgrade.hex && \
cp ./build/${keyboard}_IAP.hex ./${keyboard}_IAP.hex && \
mergehex --merge ./${keyboard}_upgrade.hex ./${keyboard}_IAP.hex --output ${keyboard}_factory.hex && \
./utils/uf2conv.py ${keyboard}_upgrade.hex -c -f 0x1aa143c7 -o ${keyboard}_upgrade.uf2 && \
rm ./${keyboard}_IAP.hex && \
rm ./${keyboard}_upgrade.hex
