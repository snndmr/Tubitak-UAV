#!/bin/sh

#
# Author : snn
# To use : sudo chmod +x /path/to/file/cv2rasp && ./cv2rasp
# To test: g++ main.cpp -o run `pkg-config --cflags --libs opencv4` && ./run [argv if there is argument]
#

sudo apt purge sonic-pi -y
sudo apt purge minecraft-pi -y
sudo apt purge libreoffice* -y

sudo apt-get update -y
sudo apt-get upgrade -y
sudo apt-get clean -y
sudo apt-get autoclean -y
sudo apt-get autoremove -y

sudo apt-get install -y build-essential
sudo apt-get install -y cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev pkg-config 

cd ~/ && git clone https://github.com/opencv/opencv.git
cd ~/opencv/ && mkdir build && cd build

cmake -DCMAKE_BUILD_TYPE=Release -DOPENCV_GENERATE_PKGCONFIG=ON -DCMAKE_INSTALL_PREFIX=/usr/local ..

make -j4
sudo make install
sudo ldconfig

reboot
