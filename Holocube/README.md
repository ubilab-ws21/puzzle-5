<<<<<<< HEAD
# Holocube
The Holocube is a pseudo-holographic transparent display desktop station with network function and hosts the game2048 app.
This directory contains the schematics, PCB, test firmware and app software for the Holocube.


## Hardware
The main board's [schematics](./hw/schematics) and [PCB](./hw/PCB/main_board/) of the Holocube consists of an ESP32, IMU sensor,
SD card slot, CP210X, RGB, light sensor, an antenna and an LDO whose functions are as follows:

| Component      | Function    |
| -------------- | ----------- |
| ESP32PICO-D4   | The main SiP providing the proccessor, RAM, Flash memory and WiFi network access |
| IMU (MPU9250)  | ...Text     |
| CP210X         | Provides the programming and debug interface for the Holocube SiP - ESP32PICO-D4 |
| RGB            | ...Text     |
| SD Card & Slot | ...Text     |
| Light Sensor   | ...Text     |
| Antenna        | ...Text     |
| LDO            | ...Text     |

The extension board's [schematics](./hw/schematics) and [PCB](./hw/PCB/extension_board/) of the Holocube consists of an LCD and a 
P-MOSFET whose fuctions are as follows:

| Component      | Function    |
| -------------- | ----------- |
| LCD            | ...Text     |
| P-MOSFET       | ...Text     |

[See more details](https://easyeda.com/fedy0/game2048)


## Software
The software `sw/tests` contains all the tests for the components of Holocube (both main and extention boards)

