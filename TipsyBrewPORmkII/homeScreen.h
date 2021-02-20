void drawPreHeatButton() {
  if (appMode == APP_HOME) {
    if (preHeatStatus) {
      drawBigButton(LEFT_ON, TOP, flame, TB_ORANGE, MOSTLY_WHITE, "Pre-heat", MOSTLY_WHITE, TB_ORANGE, "ON");
    } else {
      drawBigButton(LEFT_OFF, TOP, flame, TB_ORANGE, MOSTLY_WHITE, "Pre-heat", MOSTLY_WHITE, TB_ORANGE, "OFF");
    }
  }
}

void togglePreHeat() {
  preHeatStatus = !preHeatStatus;
  drawPreHeatButton();
  if (preHeatStatus) {
    startPreHeat();
  } else {
    cancelPreHeat();
  }
}

void switchToHome() {
  appMode = APP_HOME;
  topBarHomeScreen();
  
  brewerStateLED(READY_NOW);
  
  // clear the area under the top bar
  tft.fillRect(0, 26, tft.width(), tft.height()-26, BKGD);

  drawPreHeatButton();
  
  drawBigButton(LEFT_OFF, BOTTOM, cardList, TB_ORANGE, MOSTLY_WHITE, "Logs", MOSTLY_WHITE, GREY_GRAY, "COMING SOON");
  
  drawBigButton(RIGHT_OFF, TOP, coffeeBean, MOSTLY_WHITE, TB_ORANGE, "Coffee", TB_ORANGE, MOSTLY_WHITE);

  drawBigButton(RIGHT_OFF, BOTTOM, gear, MOSTLY_WHITE, TFT_BLUE, "Settings", TFT_BLUE, MOSTLY_WHITE);
}

/******************
 * Button Functions
 */
void homeLeftUpper() {
  togglePreHeat();
}

void homeLeftLower() {
  // Logs not yet implemented
}

void homeRightUpper() {
  // Let's make some coffee!!!
}

void homeRightLower() {
  // Settings coming soon
}
