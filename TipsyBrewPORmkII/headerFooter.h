void wifiIconOn() {
  tft.fillRect(295, 0, 25, 25, BKGD);
  tft.drawBitmap(tft.width() - 25, 0, wifi, 25, 25, GREY_GRAY);
}

void wifiIconOff() {
  tft.fillRect(tft.width() - 25, 0, 25, 25, BKGD);
  tft.drawBitmap(tft.width() - 25, 0, wifiOff, 25, 25, GREY_GRAY);
}

String getShortFormattedTime() {
  rawTime = timeClient.getEpochTime();
  ntpHours = (rawTime % 86400L) / 3600;
  String hoursStr = ntpHours < 10 ? "0" + String(ntpHours) : String(ntpHours);

  ntpMinutes = (rawTime % 3600) / 60;
  String minuteStr = ntpMinutes < 10 ? "0" + String(ntpMinutes) : String(ntpMinutes);

  return hoursStr + ":" + minuteStr;
}

void printTime() {
  topBarTime = getShortFormattedTime();
  int str_len = topBarTime.length() + 1;
  char char_array[str_len];
  topBarTime.toCharArray(char_array, str_len);
  
  tft.fillRect(100, 1, tft.width() - 200, tft.fontHeight(4), BKGD);
  tft.setTextColor(GREY_GRAY);
  tft.drawCentreString(char_array, tft.width()/2, 1, 4);
}

void topBarHomeScreen() {
  tft.fillRect(0, 0, 99, 25, BKGD);
  tft.drawBitmap(3, 0, homeSmall, 25, 25, GREY_GRAY);
  tft.drawString("Home", 28, ((25 - tft.fontHeight(2))/2), 2);
}

void footerPrintTemp() {
  tft.fillRect(0, tft.height() - 25, tft.width()/2, 25, BKGD);
  char str[14];
  dtostrf(kettleTemp, 4, 1, str);
  strcat(str, " deg C");
  tft.setTextColor(MOSTLY_WHITE);
  tft.drawString(str, 3, tft.height() - 25 + ((25-tft.fontHeight(2))/2), 2);
}

void footerPrintMsg(char *message, bool error = false) {
  tft.fillRect(tft.width()/2, tft.height() - 25, tft.width()/2, 25, BKGD);
  error ? tft.setTextColor(TFT_RED) : tft.setTextColor(MOSTLY_WHITE);
  tft.drawRightString(message, tft.width() - 3, tft.height() - 25 + ((25-tft.fontHeight(2))/2), 2);
}
