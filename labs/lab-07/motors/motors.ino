#include <ESP32Servo.h>
#include <Stepper.h>

Stepper myStepper(2048, 12, 14, 27, 26);
Servo servo;

void setup() {
  myStepper.setSpeed(5);
  servo.attach(25);
}

int servoPos = 0;
void loop() {
  servo.write(0);
  delay(700);
  servo.write(95);
  delay(700);
  servo.write(210);
  delay(700);
}
