#ifndef DISPLAY_TASK
#define DISPLAY_TASK

#include <Arduino.h>
#include <TFT_eSPI.h> // Graphics and font library for ILI9341 driver chip
#include "icons.h"

extern TFT_eSPI tft;
extern AppValues appState;

void updateDisplay(void * parameter){
    for(;;){
        // This handles all changes to the display
        // To my knowledge, splitting display updates to multiple threads could cause issues
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
                    appState.currentScreen = APP_HOME;
                }
                break;
            case APP_HOME: {
                    tft.setViewport(0, 25, 320, 190);
                    tft.fillScreen(BKGD);
                    tft.resetViewport();
                }
                break;
            case APP_SETTINGS:
                // seriously, I said baby steps!
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
        char timeHourMinute[6];
        strftime(timeHourMinute, 6, "%I:%M", &appState.timeinfo);
        tft.drawString(timeHourMinute, tft.width()/2, 13, 4);

        vTaskDelay(250 / portTICK_PERIOD_MS);
    }
}

#endif