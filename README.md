# this is a ball balanacing robot I made in 5 weeks at around 5 hours per week, mainly to prove I could do it
this is just a high level paste of all systems, if your looking for walkthrough tutorial check out these links:

https://raw.org/research/inverse-kinematics-of-a-stewart-platform/

https://www.youtube.com/watch?v=jyQyusoxlW8

my goals for this:

- learn communication between hardware
- learn PID implemenentation and practical tuning
- learn practical inverse kinematics


## Table of Contents
- [Usage](#usage)
- [Kinematics](#kinematics)
- [Mechanical Design](#mechanical_design)
- [Electrical Design](#electrical_design)
- [PID Control Design](#PIDcontrol)

## Usage
1. camera takes image
2. circle detection and location taken from image
3. put location through PID controller (x and y seperate), out put is platform angle (x and y angles)
4. send platform angles values over UART to Arduino
5. in Arduino, use platform angles to calculate resulting servo angles
6. set servo angles
Why didnt I just compute servo angles and send PWM signals to servos from my OpenMV CAM RT1062? Because I wanted to learn how to connect the boards :)
I started with SPI but this was diffucult since they needed to be in clock sync..... went to uart because I could just shove data to arduino whenever it was ready

## Kinematics
### translate frame in camera


![image](https://github.com/user-attachments/assets/9df9e9ca-ba14-47f0-bf1a-038a44bec067)

### find lengths of all 3 links

<img src="https://github.com/user-attachments/assets/eb05e406-64a8-4dbc-900c-8f0044ec2ef8" alt="Alt Text" style="width:30%; height:auto;">

### translate these lengths to angles too apply to servo

<img src="https://github.com/user-attachments/assets/71c701e1-9baf-4020-8444-788abdef5b34" alt="Alt Text" style="width:40%; height:auto;">

## Mechanical_Design

### Beta version

![image](https://github.com/user-attachments/assets/d76cc83b-b593-41ef-9617-036e46de03cb)

### Alpha version

the platform moved around so much....this was due to many bad tolerance stackups and the soft plastic....beta version fixed this using shorter c link and out of threaded rod

also the acrylic mounting piece was too small to be practical... for beta I decided to put a hole in the acrylic and properly measure 120 degrees instead of press fitting these on

<img src="https://github.com/user-attachments/assets/b495f711-b998-4229-93f0-d0f747f591f0" alt="Alt Text" style="width:40%; height:auto;">

Joints: RC ball joints: https://www.amazon.ca/dp/B0DK321WTZ?ref=ppx_yo2ov_dt_b_fed_asin_title


## Electrical_Design

very simple electical for this one

Power supply: 24V 10A DC power supply: https://www.amazon.ca/dp/B0CZ92WTQ3?ref=ppx_yo2ov_dt_b_fed_asin_title

Motors: MG995 metal gear 4.8-6V 180 deg servos: https://www.amazon.ca/dp/B07RFRLRV8?ref=ppx_yo2ov_dt_b_fed_asin_title

Camera: OpenMV CAM RT1062: https://www.amazon.ca/dp/B0CWP7LT57?ref=ppx_yo2ov_dt_b_fed_asin_title spent my coop money on this one......

Arduino: Arduino Uno R3










## PID_Control
