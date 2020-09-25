# Rover


## Introduction
This is an Arduino based RC-Car, which can move around in two modes. The Bluetooth control mode allows to control the rover through a HC-05 module via a smartphone and any Bluetooth controller app. In the pathfinder mode the rover moves around on its own and avoids obstacles with the help of a servo motor and an ultrasonic sensor.


## Code and Recources
- Environment: Arduino IDE, Visual Studio Code -> PlatformIO extension
- Libraries: Servo, NewPing, SoftwareSerial


## Build
Parts used are an Arduino Uno, Sensor Shield v5.0, L298N H-Bridge Dual DC Motor Driver, 2x Li-Ion 3.7V batteries as an external power supply, HC-05 Bluetooth Module.


## Flow
Bluetooth Mode: Control the rover through the HC-05 module via an app on the smartphone.

Pathfinder Mode: Rover moves forward until it detects an obstacle in close proximity. It then backs up, scans its environment and finally moves in the direction with the least amount of obstruction.

Below is a link to a YouTube video demonstating the movement (make sure to open the link in a new tab):


## Circuit
Pin Name | Pin Used
------------ | -------------
(left motor) ENA | 6
(left motor) IN1 | 8
(left motor) IN2 | 7
(right motor) ENB | 5
(right motor) IN3 | 4
(right motor) IN4 | 2
(servo) PWM | 3
(sonar) Trig | A1
(sonar) Echo | A0
(HC-05) TX | 9
(HC-05) RX | 10
