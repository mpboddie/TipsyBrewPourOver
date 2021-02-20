void drawPreHeatButton();   // prototyped for the homeScreen, NOTE: I don't think I like the way I did this, this may be a slippery slope
void cancelPreHeat();

void kettleOn() {
  kettleServo.attach(KETTLE_SWITCH_PIN);
  kettleServo.write(KETTLE_ON);
  delay(1000);
  kettleServo.write(KETTLE_NEUTRAL);
  delay(1000);
  kettleServo.detach();
  kettleOnEvent = millis();
}

void kettleOff() {
  kettleServo.attach(KETTLE_SWITCH_PIN);
  kettleServo.write(KETTLE_OFF);
  delay(1000);
  kettleServo.write(KETTLE_NEUTRAL);
  delay(1000);
  kettleServo.detach();
  kettleOffEvent = millis();
}

bool isKettleOn() {
  bool kettleState = false;
  if (kettleOnEvent > kettleOffEvent) { kettleState = true; }
  return kettleState;
}

void startPreHeat() {
  preHeatStatus = true;
  drawPreHeatButton();
  kettleOn();
  preHeatTimer.in(preHeatDuration, cancelPreHeat);
}

void cancelPreHeat() {
  preHeatTimer.cancel();
  preHeatStatus = false;
  kettleOff();
  drawPreHeatButton();
}
