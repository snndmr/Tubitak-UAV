#!/bin/sh

#
# Author : snn
# To use : sudo chmod +x /path/to/file/opencv_installer && ./opencv_installer
#

echo "Author : snn | Opencv Installer"

sudo apt purge sonic-pi -y
sudo apt purge minecraft-pi -y
sudo apt purge libreoffice* -y

sudo apt-get update -y
sudo apt-get upgrade -y
sudo apt-get clean -y
sudo apt-get autoclean -y
sudo apt-get autoremove -y

sudo pip install --upgrade pip
sudo apt-get install -y libatlas-base-dev libhdf5-dev libhdf5-serial-dev libatlas-base-dev libjasper-dev  libqtgui4  libqt4-test
sudo pip3 install opencv-contrib-python==4.1.0.25

reboot
