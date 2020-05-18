void displayIntro() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,10);
  display.println(F("Place a prepared"));
  display.println(F("brewer in the machine"));  
  display.println(F("WITHOUT any grounds"));
  display.display();
  delay(1000);
  display.setCursor(10,44);
  display.println(F("Press any key to"));
  display.println(F("continue."));
  display.display();
}

void displayFilterRinse() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println(F("DOWN = Rinse Filter"));
  display.println(F(" "));  
  display.println(F("When complete,"));
  display.println(F("Empty rinse water"));
  display.println(F("Press LEFT"));
  display.display();
}

void displayServoTest(int angle, int distance) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print(F("ANGLE = "));
  display.println(angle);
  display.print(F("DISTANCE = "));
  display.println(distance);
  display.display();
}
