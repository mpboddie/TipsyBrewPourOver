#ifndef TIME_TASK
#define TIME_TASK

#include <Arduino.h>

extern AppValues appState;

void updateDateTime(void * parameter){
    for(;;){
        if(appState.wifiState == WIFI_CONNECTED) {
            if(!getLocalTime(&appState.timeinfo)){
                Serial.println(F("[NTP]Failed to obtain time"));
                vTaskDelay(1000 / portTICK_PERIOD_MS);
            }
        }
        vTaskDelay(250 / portTICK_PERIOD_MS);
    }
}

#endif