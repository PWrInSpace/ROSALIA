# ROSALIA MainBoard

## Rocket Static Fire stand for Advanced Launch and Integration of Aerospace Instruments

## Software

Using [ESP-IDF v5.1 release](https://docs.espressif.com/projects/esp-idf/en/release-v5.1/esp32/).

Developed software:
- RFM95w LoRa library
- SD Card support
- External and internal flash memories
- Can transceiver (Planned)
- BLE with OTA capabilities (Planned)

### Building the project

To build the project, one can use ESP-IDF build natively or build a docker image with the use of provided Dockerfile

#### 1. Installing esp-idf and running idf.py on machine

A great documentation on this can be found on [Espressif docs page regarding the v5.1 release](https://docs.espressif.com/projects/esp-idf/en/release-v5.1/esp32/get-started/index.html#installation)

#### 2. Building a docker image and running a container

The Dockerfile in this project is nearly identical to the one provided by espressif ___espressif/esp-idf___

Prerequisites are obviously having Docker (min. ver. 24.0) installed on the machine. Then, one should run the command to build the container in ROSALIA directory:

```bash
docker build -t dockeruser/image_name:version .
```

When the image is built successfuly, run the image:
```bash
docker docker run --name rosalia --rm -v $PWD:/rosalia -w /rosalia -u $UID -e HOME=/tmp -itdockeruser/image_name:version
```

This will create and put you into a virtual environment with idf.py already running. You can then go into Mainboard_ESP32 directory and use idf.py.

**Note 1:** Outside of Linux there will be problems with using physical ports (mainly USB in this example). To overcome this, one can use [remote serial ports](https://docs.espressif.com/projects/esptool/en/latest/esptool/remote-serial-ports.html), with better instructions in [Docker documentation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/tools/idf-docker-image.html#using-remote-serial-port)

**Note 2:** some of the directories such as ___/build___ will be replaced in the **local** directory.

More info and how to run it: [esp-idf Docker image, documentation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/tools/idf-docker-image.html#usage)

### Run unit tests

To run unit tests:
```bash
cd Mainboard_ESP32/test
idf.py build
# if build does not work checkout idf.py menuconfig
idf.py flash monitor
# reset the board
```

## Hardware mainboard

The mainboard features:
- ESP32 microcontroller
    - RFM95w module for LoRa communication
    - SD Card and external flash memories
    - 2 servo controllers with limit switches
    - BLE support
- ESP32 Pico Microcontroller
    - Power sustain and remote turn-off abilities
    - Buzzer
    - Can transceiver

![Board render](Docs/MiniHamowniav3_render.png)

[Main project soundtrack](https://www.youtube.com/watch?v=jWlaaK7BxeU)