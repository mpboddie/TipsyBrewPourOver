#ifndef WIFI_CONNECTION_TASK
#define WIFI_CONNECTION_TASK

#include <Arduino.h>
#include <WiFi.h>
#include "../config/userSettings.h"

void keepWiFiAlive(void * parameter){
    for(;;){
        if(WiFi.status() == WL_CONNECTED){
            vTaskDelay(10000 / portTICK_PERIOD_MS);
            continue;
        }

        //gDisplayValues.currentState = CONNECTING_WIFI;

        WiFi.mode(WIFI_STA);
        WiFi.setHostname(hostname);
        WiFi.begin(ssid, password);

        unsigned long startAttemptTime = millis();

        // Keep looping while we're not connected and haven't reached the timeout
        while (WiFi.status() != WL_CONNECTED && 
                millis() - startAttemptTime < WIFI_TIMEOUT){}

        // Make sure that we're actually connected, otherwise go to deep sleep
        if(WiFi.status() != WL_CONNECTED){
            vTaskDelay(WIFI_RECOVER_TIME / portTICK_PERIOD_MS);
        }

        //serial_println(WiFi.localIP());
        //gDisplayValues.currentState = UP;
    }
}

#endif