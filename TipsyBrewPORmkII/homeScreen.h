bool preHeatStatus = false;

void drawPreHeatButton() {
  if (preHeatStatus) {
    drawBigButton(LEFT_ON, BOTTOM, flame, TB_ORANGE, MOSTLY_WHITE, "Pre-heat", MOSTLY_WHITE, TB_ORANGE, "ON");
  } else {
    drawBigButton(LEFT_OFF, BOTTOM, flame, TB_ORANGE, MOSTLY_WHITE, "Pre-heat", MOSTLY_WHITE, TB_ORANGE, "OFF");
  }
}

void togglePreHeat() {
  preHeatStatus = !preHeatStatus;
  drawPreHeatButton();
  if (preHeatStatus) {
    // turn on the heat
    // set a timer to turn it off, just in case the user turns on preheat and walks away we don't want all the water to evaporate and dry boil
  } else {
    // turn off heat and cancel a timer if it were on
  }
}

void switchToHome() {
  appMode = APP_HOME;
  topBarHomeScreen();
  
  brewerStateLED(READY_NOW);
  
  // clear the area under the top bar
  tft.fillRect(0, 26, tft.width(), tft.height()-26, BKGD);

  drawBigButton(LEFT_OFF, TOP, cardList, TB_ORANGE, MOSTLY_WHITE, "Logs", MOSTLY_WHITE, GREY_GRAY, "COMING SOON");

  drawPreHeatButton();
  
  drawBigButton(RIGHT_OFF, TOP, coffeeBean, MOSTLY_WHITE, TB_ORANGE, "Coffee", TB_ORANGE, MOSTLY_WHITE);

  drawBigButton(RIGHT_OFF, BOTTOM, gear, MOSTLY_WHITE, TFT_BLUE, "Settings", TFT_BLUE, MOSTLY_WHITE);
}

void homeLeftLower() {
  togglePreHeat();
}
