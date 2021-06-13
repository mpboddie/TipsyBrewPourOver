#ifndef KETTLE_TASK
#define KETTLE_TASK

#include <Arduino.h>
#include <DallasTemperature.h>

extern AppValues appState;
extern DallasTemperature sensors;

void updateKettleTemp(void * parameter){
    for(;;){
        sensors.requestTemperatures();
        double kettleTemp = sensors.getTempCByIndex(0);
        if(appState.kettleTemp != kettleTemp) {
            appState.kettleTemp = kettleTemp;
            appState.kettleChange = true;
        }
        if(appState.preheatStatus) {
            if(appState.kettleTemp < (coffeeSettings.preheatTarget-1) && !appState.kettleState) {
                // preheat is active, the kettle is a bit cool and the kettle is currently off
                appState.kettleState = true;
                // TODO: turn on the kettle
            } else if(appState.kettleTemp >= coffeeSettings.preheatTarget && appState.kettleState) {
                // preheat is active, the kettle is at temp or above and the kettle is currently on
                appState.kettleState = false;
                // TODO: turn off the kettle
            }
        } else if(appState.currentScreen == APP_COFFEE) {
            if(appState.kettleTemp < (coffeeSettings.brewTempTarget - 1) && !appState.kettleState) {
                // brewing is active, the kettle is a bit cool and the kettle is currently off
                appState.kettleState = true;
                // TODO: turn on the kettle
            } else if(appState.kettleTemp >= coffeeSettings.brewTempTarget && appState.kettleState) {
                // brewing is active, the kettle is at temp or above and the kettle is currently on
                appState.kettleState = false;
                // TODO: turn off the kettle
            }
        }
        vTaskDelay(250 / portTICK_PERIOD_MS);
    }
}

#endif