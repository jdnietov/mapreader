# MapReader

 Fetch the coordinates of accidents displayed on a Google Maps for Bogotá, Colombia.

## Getting Started

This program is specifically designed for a 1920 x 1080 screen running a 100%-zoom Google Chrome window with a 16z Google Map.

For a more complete version you can also check out [WazeReader](https://github.com/jdnietov/wazereader), which uses Waze and fetches many other events on the map.

### Prerequisites

- Bash script interpreter
- G++ compiler
- Google Chrome +59.0 (uses `--headless` flag)
- OpenCV 2.4 (for installing you can follow the [official instructions](http://docs.opencv.org/2.4/doc/tutorials/introduction/linux_install/linux_install.html) -- or use [these](https://github.com/jayrambayrambhia/Install-OpenCV) handy scripts)

## Installing

Clone the repository in your machine and run

```
cd mapReader/
make
```

## Usage

```
./mapReader [--graphic, --debug]
```

All results will be written to a `data.log` file.
- The `--graphic` flag allows to display match results on a window.
- The `--debug` flag displays every screenshot the browser fetches, regardless of the matches in it, in order to monitor what the program is using.

## Authors

* **José David Nieto Vitola** - *Initial work* - [jdnietov](https://github.com/jdnietov)

## License

This project is licensed under the MIT License.

## Acknowledgments

* [WazeReader](https://github.com/jdnietov/mapreader), a more complete version using Waze, is based upon this project (no, I didn't know what forking in Github was when I wrote these). It also fetches traffic jams, hazards and many other events. Take a look if you're interested.
