from gpiozero import Button
import os
import time

switch = Button(3, pull_up=True)
pushbutton = Button(4, pull_up=True)
joystick = Button(17, pull_up=True)

game_state = "intro" # intro, mid_game, game_over

# print("Pin 3:", switch.is_pressed)
# print("Pin 4:", pushbutton.is_pressed)
# print("Pin 17:", joystick.is_pressed)



def game_loop():
    