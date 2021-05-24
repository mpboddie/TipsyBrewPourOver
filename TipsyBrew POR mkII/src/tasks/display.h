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
                    tft.drawBitmap(9, 88, tipsyBrewTitle, 300, 62, TB_ORANGE);
                    tft.setTextColor(TFT_WHITE, TFT_BLACK);
                }
                break;
            case APP_HOME:
                // baby steps
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
        vTaskDelay(250 / portTICK_PERIOD_MS);
    }
}

#endif