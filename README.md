# SmartPlantV1
Automatic Smart Plant Pot - (DIY, 3d Printed, Arduino, Self Watering, Project)


Print, assemble and program your own intelligent Smart Plant Pot. With a hidden water reservoir, pump and sensors why not make your plant care easy.

With an Arduino Nano at its core this project uses a water level sensor to let you know when to refill the concealed tank and a solid moisture sensor to figure out when to water your plant based on its needs.

==========

List of items used in this project and where to find them (the links are Amazon affiliate links and help to support this channel at no cost to you):

- Arduino Nano: https://geni.us/ArduinoNanoV3

- Mini submersible pump: https://geni.us/MiniPump

- 5mm tubing: https://geni.us/5mmTubing

- Transistor: https://geni.us/2npn2222

- Resistors (1k and 4.7k): https://geni.us/Ufa2s

- Wire: https://geni.us/22AWGWire

- 3mm LED: https://geni.us/LEDs

- Water level sensor: https://geni.us/WaterLevelSensor

- Nuts and Bolts: http://geni.us/NutsAndBolts

- ~~Soil moisture sensor: [https://geni.us/MoistureSensor](https://geni.us/MoistureSensor)~~

- Soil moisture sensor https://www.az-delivery.de/nl/products/bodenfeuchte-sensor-modul-v1-2

- Half Perma-proto board: https://geni.us/HalfPermaProto

- PLA Filament: https://geni.us/PLAFilament


- Battery powered glue gun: http://geni.us/BoschBatteryGlueGun

- Battery powered soldering iron:  http://bit.ly/SealeySDL6

==========

3D printed parts and wiring diagram can be downloaded from here: https://www.thingiverse.com/thing:3537287

==========

SAY THANKS:

Buy me a coffee to say thanks: https://ko-fi.com/diymachines
Support us on Patreon: https://www.patreon.com/diymachines

SUBSCRIBE:
■ https://www.youtube.com/channel/UC3jc4X-kEq-dEDYhQ8QoYnQ?sub_confirmation=1

INSTAGRAM: https://www.instagram.com/diy_machines/?hl=en
FACEBOOK: https://www.facebook.com/diymachines/


## nix develop

This sketch can be developed with [Nix](https://nixos.org/download.html).

`nix develop` provides a semi-reproducible development environment: `arduino-cli` and other packages are pinned, but `arduino-cli` downloads its own packages. These are in turn pinned via the [sketch.yaml](sketch.yaml) file (available in `arduino-cli` version 0.23 and above).

For NixOS: make sure the user is in the "dialout" group to access the serial port.

```nix
users.users.<user>.extraGroups = [ "dialout" ];
```

### arduino-cli

Use `make` for some predefined options to compile and upload the sketches.

Hints to compile / upload from command line:

* https://stackoverflow.com/questions/57429802/is-there-a-way-to-use-arduino-cli-to-compile-sketch-ino-to-a-bin-file
* https://create.arduino.cc/projecthub/B45i/getting-started-with-arduino-cli-7652a5
* https://arduino.github.io/arduino-cli/0.25/sketch-project-file/

```console
$ arduino-cli board list
$ arduino-cli board listall  # Get fqbn name from here
$ arduino-cli compile --fqbn arduino:avr:nano SmartPlantV1
$ arduino-cli upload -p /dev/ttyUSB0 --fqbn arduino:avr:nano SmartPlantV1
$ screen /dev/ttyUSB0 9600  # Use the output from the 'list' command to determine the TTY, 9600 is set in the sketch
```
