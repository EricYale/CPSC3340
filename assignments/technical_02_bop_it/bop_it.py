import os
import time
import pygame
from gpiozero import Button
import random

TEMPO = 150  # BPM
BGM_BEATS = 16
FRAMERATE = 120.0

switch = Button(3, pull_up=True)
pushbutton = Button(4, pull_up=True)
joystick = Button(17, pull_up=True)

game_state = "intro" # intro, mid_game, game_over
t = 0

pygame.mixer.init()
bgm_length_ms = int(pygame.mixer.Sound(os.path.join("resources", "bgm.wav")).get_length() * 1000)

def play_bgm():
    pygame.mixer.music.load(os.path.join("resources", "bgm.wav"))
    pygame.mixer.music.play(-1)

def stop_bgm():
    pygame.mixer.music.stop()

def play_sound(filename):
    sound = pygame.mixer.Sound(os.path.join("resources", filename))
    sound.play()

def loop_intro():
    global t
    if t > 2.5:
        play_sound("voice_start.wav")
        t = 0
    if joystick.is_pressed:
        state_transition("mid_game")

SAMPLES = [
    "voice_bopit.wav",
    "voice_flickit.wav",
    "voice_pressit.wav"
]

last_beat_was_downbeat = False


def loop_game():
    global last_beat_was_downbeat
    music_pos_ms = pygame.mixer.music.get_pos()
    if music_pos_ms < 0:
        return  # music not playing
    # Use modulo to get position within the current loop
    music_pos_ms = music_pos_ms % bgm_length_ms
    beat_length_ms = 1000 * 60 / TEMPO
    
    if music_pos_ms < beat_length_ms * 8 and (not last_beat_was_downbeat):
        last_beat_was_downbeat = True
        play_sound(random.choice(SAMPLES))
    elif music_pos_ms > beat_length_ms * 8 and last_beat_was_downbeat:
        last_beat_was_downbeat = False
        play_sound(random.choice(SAMPLES))
    

def loop_game_over():
    stop_bgm()
    print("In game over loop")

def loop():
    global t, game_state
    if game_state == "intro":
        loop_intro()
    elif game_state == "mid_game":
        loop_game()
    elif game_state == "game_over":
        loop_game_over()
    t += 1 / FRAMERATE

def state_transition(new_state):
    global game_state, t
    game_state = new_state
    t = 0

    if new_state == "intro":
        stop_bgm()
    elif new_state == "mid_game":
        play_bgm()
    elif new_state == "game_over":
        stop_bgm()

def main():
    state_transition("intro")
    while True:
        loop()
        time.sleep(1 / FRAMERATE)

main()
