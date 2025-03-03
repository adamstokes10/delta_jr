# This work is licensed under the MIT license.
# Copyright (c) 2013-2024 OpenMV LLC. All rights reserved.
# https://github.com/openmv/openmv/blob/master/LICENSE
#
# UART Control
#
# This example shows how to use the serial port on your OpenMV Cam. Attach pin
# P4 to the serial input of a serial LCD screen to see "Hello World!" printed
# on the serial LCD display.

import time
from machine import UART
import sensor
import math
import machine

#machine.reset()


sensor.reset()
sensor.set_pixformat(sensor.GRAYSCALE)  # grayscale is faster RGB565
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time=2000)
clock = time.clock()
uart = UART(1, 115200, timeout_char=200)

class PID:
        def __init__(self, kp, ki, kd, setpoint, output_limits=(None, None)):
            self.kp = kp
            self.ki = ki
            self.kd = kd
            self.setpoint = setpoint
            self.output_limits = output_limits

            self.last_error = 0
            self.integral = 0
            self.last_time = time.ticks_ms() / 1000

        def compute(self, measurement, current_time):
            error = self.setpoint - measurement
            delta_time = (current_time - self.last_time) if self.last_time else 0

            # Proportional term
            p_term = self.kp * error

            # Integral term
            if delta_time > 0:
                self.integral += error * delta_time
            i_term = self.ki * self.integral

            # Derivative term
            d_term = 0
            if delta_time > 0:
                d_term = self.kd * (error - self.last_error) / delta_time

            # Compute output
            output = p_term + i_term + d_term

            # Apply output limits
            if self.output_limits[0] is not None:
                output = max(self.output_limits[0], output)
            if self.output_limits[1] is not None:
                output = min(self.output_limits[1], output)

            # Store values for next iteration
            self.last_error = error
            self.last_time = current_time

            return output

def round_for_sending(value):
    sender = 0
    if value > 0:
        sender = round(value, 1)
        sender = sender*10

    elif value < 0:
        sender = round(value, 1)
        sender = abs(sender)*10 + 50

    return int(sender)

def calibration(xmeas,ymeas,xa,ya,xt,yt,xangle,yangle):
#uncomment what you want to calibrate
    print("center calibration: ",xmeas,ymeas)
    #print("rotated camera frame distances: ",xa,ya)
    #print("transmit values: ",xt,yt)
    #print("PID output: ",xangle,yangle)


P = 0.1
D = 0.015
I = 0.005
#D = 0.05
#I = 0.02
x_center = 160
y_center = 105
angle_comp = 69 * math.pi / 180

x_meas = 0
y_meas = 0

x_pid = PID(kp=P, ki=I, kd=D, setpoint=0, output_limits=(-5, 5))
y_pid = PID(kp=P, ki=I, kd=D, setpoint=0, output_limits=(-5, 5))

sw = 240

while True:
    clock.tick()
    img = sensor.snapshot()

    #find circles
    for c in img.find_circles(roi=(int((320-sw)/2),int((240-sw)/2),sw,sw),threshold=2000,x_margin=10,y_margin=10,r_margin=10,r_min=48,r_max=52,r_step=2):
        img.draw_circle(c.x(), c.y(), c.r(), color=(255, 0, 0))
        x_meas = c[0]
        y_meas = c[1]
        print(c)


    #angle comp (-21 deg)
    x_a = -(math.cos(angle_comp) * (x_meas-x_center) + math.sin(angle_comp) * (y_meas-y_center))
    y_a = -(-math.sin(angle_comp) * (x_meas-x_center) + math.cos(angle_comp) * (y_meas-y_center))


    #PID controller
    current_time = time.ticks_ms() / 1000
    x_angle = x_pid.compute(y_a,current_time)
    y_angle = y_pid.compute(x_a,current_time)

    #prep for uart
    x_t = round_for_sending(x_angle)
    y_t = round_for_sending(y_angle)


    #uncomment to calibrate something
    calibration(x_meas,y_meas,x_a,y_a,x_t,y_t,x_angle,y_angle)

    #send via uart
    tm_1 = 15
    tm_2 = 90
    #uart.write(bytearray([125],[x_t],[y_t],[126]))
    uart.write(bytearray([125, x_t, y_t, 126]))
    #print(x_t,y_t,"\n")
    #uart.write(bytes([126]))
    #time.sleep(0.1)
    #uart.write(bytes([x_t]))
    #time.sleep(0.1)
    #uart.write(bytes([y_t]))
    #time.sleep(0.1)



