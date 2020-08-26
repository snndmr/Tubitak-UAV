#!/bin/sh

#
# Author : snn
# To use : sudo chmod +x /path/to/file/pymavlink_installer && ./pymavlink_installer
#

echo "Author : snn | Pymavlink Installer"


sudo apt-get update -y
sudo apt-get upgrade -y
sudo apt-get clean -y
sudo apt-get autoclean -y
sudo apt-get autoremove -y

sudo apt-get install gcc python-dev libxml2-dev libxslt-dev
sudo apt-get install python-numpy python-pytest
sudo pip3 install -U future lxml
sudo pip3 install -U pymavlink

reboot
