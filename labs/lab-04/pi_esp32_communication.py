import time
import serial

ser = serial.Serial("/dev/ttyUSB0", 115200)

while True:
    # Read lines from serial until there's none left
    while ser.in_waiting:
        line = ser.readline().decode('utf-8').rstrip()
        print(line)
    time.sleep(0.01)
