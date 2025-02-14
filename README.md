# this is a robot I made in my 4B term at school, I did this in 5 weeks at around 5 hours per week, mainly to prove I wasnt an idiot

## Table of Contents
- [Installation](#installation)
- [Usage](#usage)
- [Kinematics](#kinematics)
- [Mechanical Design](#mechanical_design)
- [Electrical Design](#electrical_design)

## Pipeline
1. camera takes image
2. circle detection and location taken from image
3. put location through PID controller (x and y seperate), out put is platform angle (x and y angles)
4. send platform angles values over UART to Arduino
5. in Arduino, use platform angles to calculate resulting servo angles
6. set servo angles
Why didnt I just compute servo angles and send PWM signals to servos from my OpenMV CAM RT1062? Because I wanted to learn how to connect the boards :)
I started with SPI but this was diffucult since they needed to be in clock sync..... went to uart because I could just shove data to arduino whenever it was ready

## Kinematics
###translate frame in camera

![image](https://github.com/user-attachments/assets/9df9e9ca-ba14-47f0-bf1a-038a44bec067)

###find lengths of all 3 links

![image](https://github.com/user-attachments/assets/eb05e406-64a8-4dbc-900c-8f0044ec2ef8)

###translate these lengths to angles too apply to servo

![IMG_3162](https://github.com/user-attachments/assets/71c701e1-9baf-4020-8444-788abdef5b34)


##Mechanical_Design

###Beta version
![image](https://github.com/user-attachments/assets/d76cc83b-b593-41ef-9617-036e46de03cb)

###Alpha version
![IMG_0939](https://github.com/user-attachments/assets/b495f711-b998-4229-93f0-d0f747f591f0)
