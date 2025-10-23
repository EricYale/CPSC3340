# Lab 7: Motors

![Wiring](./resources/wiring.jpg)

We used [this guide](https://randomnerdtutorials.com/esp32-stepper-motor-28byj-48-uln2003/) for stepper motor wiring reference, and [this guide](https://docs.arduino.cc/tutorials/generic/basic-servo-control/) for the servo.

One challenge was deciding which ESP32-compatible servo library to use. We settled on `ESP32Servo` (binary included in repo), version 3, which seemed to work.

Code in `motors/motors.ino`
