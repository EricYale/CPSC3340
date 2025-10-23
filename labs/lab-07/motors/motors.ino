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
  myStepper.step(2048);
  servo.write(servoPos);
  servoPos += 10;

  if(servoPos > 180) {
    servoPos = 180;
    servo.write(0);
  }

  delay(15);
}
