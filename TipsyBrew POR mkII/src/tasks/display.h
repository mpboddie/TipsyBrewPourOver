#ifndef DISPLAY_TASK
#define DISPLAY_TASK

#include <Arduino.h>
#include <TFT_eSPI.h> // Graphics and font library for ILI9341 driver chip
#include "icons.h"
#include "../functions/buttons.h"

extern TFT_eSPI tft;
extern AppValues appState;

void clearContentArea() {
    tft.setViewport(0, 25, 320, 190);
    tft.fillScreen(BKGD);
    tft.resetViewport();
}

void drawScreenLabel() {
  tft.fillRect(0, 0, 99, 25, BKGD);
  tft.setTextColor(GREY_GRAY, BKGD);
  tft.setTextDatum(ML_DATUM);
  switch (appState.currentScreen) {
    case APP_HOME :
        tft.drawBitmap(3, 0, homeSmall, 25, 25, GREY_GRAY);
        tft.drawString("Home", 32, 13, 2);
        break;
    case APP_SETTINGS :
        tft.drawBitmap(3, 0, gearSmall, 25, 25, GREY_GRAY);
        tft.drawString("Settings", 32, 13, 2);
        break;
    case APP_COFFEE :
        tft.drawBitmap(3, 0, coffeeSmall, 25, 25, GREY_GRAY);
        tft.drawString("Coffee", 32, 13, 2);
        break;
  }
}

void updateDisplay(void * parameter){
    for(;;){
        // This handles all changes to the display
        // To my knowledge, splitting display updates to multiple threads could cause issues
        uint16_t x, y;

        switch (appState.currentScreen)
        {
            case APP_INTRO_SCREEN: {
                    tft.drawBitmap(9, 78, tipsyBrewTitle, 300, 62, TB_ORANGE);
                    tft.setTextColor(TFT_WHITE, TFT_BLACK);
                    tft.setTextDatum(MC_DATUM);
                    int gap = 300/strlen(titleSlogan);
                    bool picked[strlen(titleSlogan)];
                    for(int j = 0; j < strlen(titleSlogan); j++) {
                        picked[j] = false;
                    }
                    int rando;
                    for(int i = 0; i < strlen(titleSlogan); i++) {
                        do {
                            rando = random(strlen(titleSlogan));
                        } while (picked[rando]);
                        tft.drawChar(titleSlogan[rando], 20+(gap*rando), 145);
                        picked[rando] = true;
                        vTaskDelay(300 / portTICK_PERIOD_MS);
                    }
                    tft.drawString(tbVersion, 160, 200);
                    vTaskDelay(1000 / portTICK_PERIOD_MS);
                    appState.screenRefresh = true;
                    appState.currentScreen = APP_HOME;
                }
                break;
            case APP_HOME: {
                    if(appState.screenRefresh) {
                        appState.screenRefresh = false;

                        clearContentArea();
                        
                        drawScreenLabel();

                        if(appState.preheatStatus) {
                            drawBigButton(LEFT_ON, TOP, flame, TB_ORANGE, MOSTLY_WHITE, "Preheat", MOSTLY_WHITE, TB_ORANGE, "ON");
                        } else {
                            drawBigButton(LEFT_OFF, TOP, flame, TB_ORANGE, MOSTLY_WHITE, "Preheat", MOSTLY_WHITE, TB_ORANGE, "OFF");
                        }
                        drawBigButton(LEFT_OFF, BOTTOM, cardList, TB_ORANGE, MOSTLY_WHITE, "Logs", MOSTLY_WHITE, GREY_GRAY, "COMING SOON");
                        drawBigButton(RIGHT_OFF, TOP, coffeeBean, MOSTLY_WHITE, TB_ORANGE, "Coffee", TB_ORANGE, MOSTLY_WHITE);
                        drawBigButton(RIGHT_OFF, BOTTOM, gear, MOSTLY_WHITE, TFT_BLUE, "Settings", TFT_BLUE, MOSTLY_WHITE);
                    }

                    if (tft.getTouch(&x, &y))
                    {
                        if(x < tft.width()/2) {
                            if(y < tft.height()/2) {
                                // Top Left Pressed
                                if(millis() - appState.activityTimer > DEBOUNCE_MS) {
                                    appState.activityTimer = millis();
                                    appState.preheatStatus = !appState.preheatStatus;
                                    //appState.screenRefresh = true;
                                    if(appState.preheatStatus) {
                                        drawBigButton(LEFT_ON, TOP, flame, TB_ORANGE, MOSTLY_WHITE, "Preheat", MOSTLY_WHITE, TB_ORANGE, "ON");
                                    } else {
                                        drawBigButton(LEFT_OFF, TOP, flame, TB_ORANGE, MOSTLY_WHITE, "Preheat", MOSTLY_WHITE, TB_ORANGE, "OFF");
                                    }
                                }
                            } else {
                                // Bottom Left Pressed
                                // Logs is not implemented yet
                            }
                        } else {
                            if(y < tft.height()/2) {
                                // Top Right Pressed
                                if(millis() - appState.activityTimer > DEBOUNCE_MS) {
                                    appState.activityTimer = millis();
                                    appState.currentScreen = APP_COFFEE;
                                    appState.screenRefresh = true;
                                }
                            } else {
                                // Bottom Right Pressed
                                if(millis() - appState.activityTimer > DEBOUNCE_MS) {
                                    appState.activityTimer = millis();
                                    appState.currentScreen = APP_SETTINGS;
                                    appState.screenRefresh = true;
                                }
                            }
                        }
                    }
                }
                break;
            case APP_SETTINGS:
                if(appState.screenRefresh) {
                    appState.screenRefresh = false;
                    clearContentArea();
                    drawScreenLabel();
                    tft.setTextColor(MOSTLY_WHITE, TFT_RED);
                    tft.setTextDatum(TC_DATUM);
                    tft.drawString("Settings not created yet", tft.width()/2, 60, 4);
                    tft.drawString("Tap to return Home", tft.width()/2, tft.fontHeight(4)+70, 4);
                }

                if (tft.getTouch(&x, &y))
                {
                    if(millis() - appState.activityTimer > DEBOUNCE_MS) {
                        appState.activityTimer = millis();
                        appState.currentScreen = APP_HOME;
                        appState.screenRefresh = true;
                    }
                }
                break;
            case APP_COFFEE:
                if(appState.screenRefresh) {
                    appState.screenRefresh = false;
                    clearContentArea();
                    drawScreenLabel();
                    tft.setTextColor(MOSTLY_WHITE, TFT_RED);
                    tft.setTextDatum(TC_DATUM);
                    tft.drawString("This is coming soon", tft.width()/2, 60, 4);
                    tft.drawString("Tap to return Home for now", tft.width()/2, tft.fontHeight(4)+70, 4);
                }

                if (tft.getTouch(&x, &y))
                {
                    if(millis() - appState.activityTimer > DEBOUNCE_MS) {
                        appState.activityTimer = millis();
                        appState.currentScreen = APP_HOME;
                        appState.screenRefresh = true;
                    }
                }
                break;
            case APP_LOGS:
                // HA! Logs?! no we aren't there yet
                // send TipsyBrew some support, I am doing this in my spare time
                break;
            default:
                break;
        }
        
        if(appState.wifiState == WIFI_CONNECTED) {
            /*if(appState.wifiStrength >= -50) {
                tft.drawString("Excellent", 10, 30);
            } else if(appState.wifiStrength >= -60) {
                tft.drawString("Good", 10, 30);
            } else if(appState.wifiStrength >= -70) {
                tft.drawString("OK", 10, 30);
            } else {
                tft.drawString("Ummmm, needs some work", 10, 30);
            }*/
            tft.drawBitmap(tft.width() - 25, 0, wifi, 25, 25, GREY_GRAY);
        } else if(appState.wifiState == WIFI_CONNECTING) {
            tft.fillRect(295, 0, 25, 25, BKGD);
        } else if(appState.wifiState == WIFI_DISCONNECTED) {
            tft.drawBitmap(tft.width() - 25, 0, wifiOff, 25, 25, GREY_GRAY);
        }

        // Draw the time
        tft.resetViewport();
        tft.setTextDatum(MC_DATUM);
        tft.setTextColor(GREY_GRAY, BKGD);
        char timeHourMinute[8];
        strftime(timeHourMinute, 8, " %I:%M ", &appState.timeinfo);
        tft.drawString(timeHourMinute, tft.width()/2, 13, 4);

        // Draw the kettle temp
        if(appState.kettleChange) {
            tft.setTextDatum(TL_DATUM);
            tft.fillRect(0, tft.height() - 25, tft.width()/2, 25, BKGD);
            char str[14];
            dtostrf(appState.kettleTemp, 4, 1, str);
            strcat(str, " deg C");
            tft.setTextColor(GREY_GRAY);
            tft.drawString(str, 3, tft.height() - 25 + ((25-tft.fontHeight(2))/2), 2);
            appState.kettleChange = false;
        }

        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

#endif