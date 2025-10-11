# Lab Notes (10/9/2025)

## Steps

- Hardware: made a simple voltage divider circuit with photoresistor and normal resistor
- Found [this documentation page](https://docs.arduino.cc/libraries/wifi/) for the arduino WiFi package
- Found [this stack overflow post](https://stackoverflow.com/a/27893987) on making a udp server in python:
- Wrote ESP32 code to connect to wifi
    - Needed to register device with yale wireless
    - Needed to loop while waiting for esp to connect
    - Needed to use `WIFI_MODE_STA`
- Wrote udp server code in python
    - `socket.socket(socket.AF_INET, socket.SOCK_DGRAM)`  creates a UDP socket
    - Had to decode messages from binary to characters
