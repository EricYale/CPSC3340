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
    if t > 3.5:
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
last_input_was_downbeat = False

last_switch_state = False

has_flicked_switch = False
has_pressed_button = False
has_joysticked = False
last_sample_played = None


def player_did_input():
    global last_sample_played, has_flicked_switch, has_pressed_button, has_joysticked
    if last_sample_played == "voice_bopit.wav" and has_joysticked:
        return True
    if last_sample_played == "voice_flickit.wav" and has_flicked_switch:
        return True
    if last_sample_played == "voice_pressit.wav" and has_pressed_button:
        return True
    return False
    
def play_sample():
    global has_flicked_switch, has_pressed_button, has_joysticked, last_sample_played
    last_sample_played = random.choice(SAMPLES)
    play_sound(last_sample_played)
    has_flicked_switch = False
    has_pressed_button = False
    has_joysticked = False


def loop_game():
    global last_beat_was_downbeat, last_switch_state, has_flicked_switch, has_pressed_button, has_joysticked, last_input_was_downbeat
    music_pos_ms = pygame.mixer.music.get_pos()
    if music_pos_ms < 0:
        return  # music not playing
    # Use modulo to get position within the current loop
    music_pos_ms = music_pos_ms % bgm_length_ms
    beat_length_ms = 1000 * 60 / TEMPO
    
    if music_pos_ms < beat_length_ms * 8 and (not last_beat_was_downbeat):
        play_sample()
        last_beat_was_downbeat = True
    elif music_pos_ms > beat_length_ms * 8 and last_beat_was_downbeat:
        play_sample()
        last_beat_was_downbeat = False

    if music_pos_ms > beat_length_ms * 4 and music_pos_ms < beat_length_ms * 8 and (not last_input_was_downbeat):
        last_input_was_downbeat = True
        if not player_did_input():
            state_transition("game_over")
    elif music_pos_ms > beat_length_ms * 12 and last_input_was_downbeat:
        last_input_was_downbeat = False
        if not player_did_input():
            state_transition("game_over")
    
    if not switch.is_pressed == last_switch_state and not has_flicked_switch:
        last_switch_state = switch.is_pressed
        has_flicked_switch = True
        print("Player flicked switch")
    if pushbutton.is_pressed and not has_pressed_button:
        has_pressed_button = True
        print("Player pressed button")
    if joystick.is_pressed and not has_joysticked:
        has_joysticked = True
        print("Player moved joystick")

def loop_game_over():
    stop_bgm()
    global t
    if t > 2.5:
        state_transition("intro")
    t += 1 / FRAMERATE

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
        play_sound("voice_lose.wav")

def main():
    state_transition("intro")
    while True:
        loop()
        time.sleep(1 / FRAMERATE)

main()
