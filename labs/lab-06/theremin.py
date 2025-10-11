import socket
import numpy as np
import sounddevice as sd
import threading
import time

SR = 44100
PLAY_TRIADS = True
SCALE = [261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88, 523.25, 587.33, 659.25, 698.46, 783.99]
TRIADS = [[0, 2, 4], [1, 3, 5], [2, 4, 6], [3, 5, 7], [4, 6, 8], [5, 7, 9], [6, 8, 10], [7, 9, 11]]
ATTACK, DECAY, SUSTAIN, RELEASE, DURATION = 0.01, 0.05, 0.6, 0.3, 0.5
THRESHOLD = 4000

freqs = [SCALE[0]]
playing = False
start = 0
above_threshold = False
phase = 0
lock = threading.Lock()

def map_freq(h):
    h = max(1900, min(3400, h))
    normalized = (h - 1900) / 1500
    curved = normalized ** 2
    i = int(curved * 7 + 0.5)
    i = max(0, min(7, i))
    if PLAY_TRIADS:
        return [SCALE[j] for j in TRIADS[i]]
    return [SCALE[i]]

def envelope(t):
    if t < ATTACK: return t / ATTACK
    if t < ATTACK + DECAY: return 1 - (1 - SUSTAIN) * (t - ATTACK) / DECAY
    if t < DURATION - RELEASE: return SUSTAIN
    if t < DURATION: return SUSTAIN * (1 - (t - DURATION + RELEASE) / RELEASE)
    return 0

def callback(out, frames, _, status):
    global phase, playing, start
    with lock:
        f, p, s = freqs, playing, start
    if p:
        elapsed = time.time() - s
        t = (np.arange(frames) + phase) / SR
        wave = sum(0.4 / len(f) * np.sin(2 * np.pi * freq * t) for freq in f) * np.array([envelope(elapsed + i / SR) for i in range(frames)])
        if elapsed >= DURATION:
            with lock: playing = False
        phase += frames
    else:
        wave = np.zeros(frames)
        phase = 0
    out[:, 0] = wave

def trigger():
    global playing, start
    with lock:
        start, playing = time.time(), True

def listen():
    global freqs, above_threshold
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s.bind(('', 7531))
    while True:
        try:
            msg = s.recvfrom(1024)[0].decode().strip()
            print(msg)
            parts = msg.split("/")
            with lock: freqs = map_freq(int(parts[0]))
            piezo = int(parts[1])
            if piezo > THRESHOLD and not above_threshold:
                trigger()
                above_threshold = True
            elif piezo <= THRESHOLD:
                above_threshold = False
        except: pass

threading.Thread(target=listen, daemon=True).start()
with sd.OutputStream(channels=1, callback=callback, samplerate=SR, blocksize=1024):
    threading.Event().wait()
