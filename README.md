# BECOv2
This repo is aimed for RoboLAND and experiment on Multi-Agent Robot

The repo is divided into three parts: real, simulation and analysis. 

### Real 
This part is directly communicating with our actual rebot. The controller we are using is Mega 2560 which supports rx/tx and pwm control.

Before running the controller, please read followings:

#### A. Wiring 
![Wiring Diagram](figs/wiring.jpg)
There are two major connection: 
1. UART(RX/TX) with LSS motor 
    **Important**: please double check the wire direction of the power and ground of LSS wire: https://wiki.lynxmotion.com/info/wiki/lynxmotion/view/ses-v2/lynxmotion-smart-servo/lss-electrical/
    !<img src="figs/LSS_Wiring.jpg" width="200"/> 
    We used serial1 from Mega 2560 to comminucate with the motor which refers to pin 18() and pin 19() on board
2. PWM with linear actuator.
    We used pin4 on PWM section to connect with the lss motor, notice the blue wire is corresponding blue wire on the lienar actuator.
    !<img src="figs/Blue_PWM.jpg" width="200"/> 
All the wiring directions are marked, please check the marker.
!<img src="figs/Mark1.jpg" width="200"/> 
#### B. Power
1. Power should be connected to the PDB with maxium 12V and minimum 6V. And current should never exceed 1A, 650mA is enough
2. Power for linear actuator and motor are both 12 V.
#### B. Coding
1. Download platformIO from vscode extensions
2. Main file in src, use platformIO to compile (main.cpp is the interface with platform io, if you want to run different code, simply copy code into main.cpp or modify compiling path in platformio.ini)

### Simulation
This part is used for robot simulation. We used the Webots and Gazebo as simulation platform.

### Analysis
All analysis code and documentation can be found here 