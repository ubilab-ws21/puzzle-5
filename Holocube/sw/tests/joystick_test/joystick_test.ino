#define JOYSTICK_XOUT 26
#define JOYSTICK_YOUT 27
#define JOYSTICK_SEL 14

void setup() {
  Serial.begin(115200);
  pinMode(JOYSTICK_XOUT, INPUT);
  pinMode(JOYSTICK_YOUT, INPUT);
  pinMode(JOYSTICK_SEL, INPUT_PULLUP);
}

void loop() {
  int xValue = analogRead(JOYSTICK_XOUT);
  int yValue = analogRead(JOYSTICK_YOUT);
  int sel = digitalRead(JOYSTICK_SEL);
  Serial.printf("X: %d | Y: %d | sel: %d\n", xValue, yValue, sel);

  if(yValue > 3000) {
    Serial.println("UP");
  }
  else if (yValue <1000) {
    Serial.println("DOWN");
  }
  else if (xValue > 3000) {
    Serial.println("RIGHT");
  }
  else if (xValue < 1000) {
    Serial.println("LEFT");
  }

  if (!sel)
    Serial.println("PRESSED");
  delay(100);
}
