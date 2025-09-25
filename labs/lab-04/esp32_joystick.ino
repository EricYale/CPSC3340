void setup() {
  Serial.begin(115200);
}

void loop() {
  int joystick = analogRead(25);
  Serial.println(joystick);
}
