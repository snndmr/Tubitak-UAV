# TUBITAK UAV ~ TEKNOFEST 2020

A code developed for the TUBITAK 5th international unmanned aerial vehicle competition.

## Installations

You can find the installation files under python/installation path.

```bash
sudo chmod +x /path/to/file/opencv_installer.sh && ./opencv_installer.sh
sudo chmod +x /path/to/file/pymavlink_installer.sh && ./pymavlink_installer.sh
```

### Usage
#### Usage without arguments
First, you need to specify the points and their properties through the data.json file.

```bash
python{your-version} /path/to/file/main.py
```

#### Usage with arguments
```bash
python{your-version} /path/to/file/main.py --camera 0
python{your-version} /path/to/file/main.py --takeoff 0
python{your-version} /path/to/file/main.py --connect /dev/serial0
python{your-version} /path/to/file/main.py --connect /dev/serial0 --camera -1
...
```

## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

Please make sure to update tests as appropriate.

## License
[MIT](https://choosealicense.com/licenses/mit/)
