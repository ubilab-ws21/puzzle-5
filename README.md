# Puzzle-5
The puzzle is a part of the Ubiquitous Lab course WS 21/22. The puzzle will be integrated in the escape room which is modelled as a control room of a power station in the future (~2050). 
## Story Overview
A Solar storm hits the city which causes a power outage at a massive scale. A group of interns/visitors are struck in the control room and have to solve a bunch of puzzles to fix the power supply and to get out of the power station eventually.
## The Puzzle
The motive of our puzzle to procure the nuclear battery from the nearby station and charge it wirelessly in a charging dock. In order to open the most technologically advanced charging dock the players will require to solve a puzzle which is centered around the famous 2048 game which will be solved using a HoloCubic. The 2048 game can be played [here](https://2048game.com/). The general flow of the puzzle and battery charging is as shown.

<img src="https://github.com/ubilab-ws21/puzzle-5/blob/main/docs/Presentations/flowdiag.jpg" width="50%"/> 

The proposed model of Holocubic for the puzzle is showcased below.

<img src="https://github.com/ubilab-ws21/puzzle-5/blob/main/docs/Presentations/Holo3.png" width="50%" />
    

## Hardware used
- MCU - ESP32-PICO-D4
- IMU - MPU6050
- Light sensor
- LED light
- Prisms and LCD 
- NFC cards and reader

## Charging dock and Batteries 
The futuristic nuclear batteries will be designed to charge in few seconds and will have contain NFC tags on them. The charging shown below will be able to read the batteries using a NFC reader.

<img src="https://github.com/ubilab-ws21/puzzle-5/blob/main/docs/Presentations/Batterybox.jpg" width="30%" />
