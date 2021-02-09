int homeMenuPos = 0;


void switchToHome() {
  appMode = APP_HOME;

  brewerStateLED(READY_NOW);
  
  // clear the area under the top bar
  tft.fillRect(0, 26, tft.width(), tft.height()-26, BKGD);

  drawBigButton(LEFT_COL, TOP, cardList, TB_ORANGE, MOSTLY_WHITE, "Logs", MOSTLY_WHITE, GREY_GRAY, "Work in progress");
  
  drawBigButton(LEFT_COL, BOTTOM, flame, TB_ORANGE, MOSTLY_WHITE, "Pre-heat", MOSTLY_WHITE, TB_ORANGE);

  drawBigButton(RIGHT_COL, TOP, coffeeBean, MOSTLY_WHITE, TB_ORANGE, "Coffee", TB_ORANGE, MOSTLY_WHITE);

  drawBigButton(RIGHT_COL, BOTTOM, gear, MOSTLY_WHITE, TFT_BLUE, "Settings", MOSTLY_WHITE, TFT_BLUE);
}
