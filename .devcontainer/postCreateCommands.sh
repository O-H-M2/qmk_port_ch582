#!/bin/zsh zsh

repo=$(pwd)

sudo apt remove -y cmake && sudo apt update && sudo apt upgrade -y && sudo apt install -y ninja-build &&
    cd ~ && wget https://github.com/Kitware/CMake/releases/download/v3.25.0/cmake-3.25.0-linux-x86_64.sh && sudo chmod +x cmake-3.25.0-linux-x86_64.sh && sudo ./cmake-3.25.0-linux-x86_64.sh --skip-license --exclude-subdir --prefix=/usr/local

cd ~/.oh-my-zsh/custom/plugins && git clone https://github.com/zsh-users/zsh-autosuggestions.git && git clone https://github.com/zsh-users/zsh-syntax-highlighting.git &&
    sed -i 's/plugins=(git)/plugins=(git zsh-autosuggestions zsh-syntax-highlighting)/' ~/.zshrc

cd ~ && wget https://www.nordicsemi.com/-/media/Software-and-other-downloads/Desktop-software/nRF-command-line-tools/sw/Versions-10-x-x/10-18-1/nrf-command-line-tools-10.18.1_Linux-amd64.tar.gz &&
    tar -zxvf nrf-command-line-tools-10.18.1_Linux-amd64.tar.gz && rm -f nrf-command-line-tools-10.18.1_Linux-amd64.tar.gz &&
    rm -f JLink_Linux_V780c_x86_64.deb && rm -f JLink_Linux_V780c_x86_64.rpm && rm -f JLink_Linux_V780c_x86_64.tgz && rm README.txt &&
    sudo ln -s ~/nrf-command-line-tools/bin/mergehex /bin/mergehex

sudo rm /bin/sh && sudo ln -s /bin/zsh /bin/sh

cd ${repo} && git -c submodule."qmk_porting/keyboards_private".update=none submodule update --recursive --init

echo "Configuration done!"
