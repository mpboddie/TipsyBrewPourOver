void initLoadingScreen() {
  tft.setTextColor(TFT_WHITE);  
  tft.drawBitmap(9, 88, tipsyBrewTitle, 300, 62, TB_ORANGE);
  wifiIconOff();
}

void loadingText(char *message) {
  tft.setTextColor(TFT_WHITE, BKGD);
  int yCursor = tft.height() - tft.fontHeight(2) - 2;
  int xCursor = 0;
  tft.fillRect(xCursor, yCursor, tft.width(), tft.height(), BKGD);
  tft.drawString(message, xCursor, yCursor, 2);
}

void clearLoadingText() {
  int yCursor = tft.height() - tft.fontHeight(2) - 2;
  int xCursor = 0;
  tft.fillRect(xCursor, yCursor, tft.width(), tft.height(), BKGD);
}

void fadeOutLoading() {
  int red = 255;
  int green = 68;
  while (red > 100)
  {
    red -= 5;
    if (green > 5) {
      green -= 5;
    } else {
      green = 0;
    }
    uint16_t color = tft.color565(red, green, 0);
    tft.drawBitmap(9, 88, tipsyBrewTitle, 300, 62, color);
    delay(5);
  }
  tft.fillRect(9, 88, 300, 62, BKGD);
}
