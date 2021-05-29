#ifndef KETTLE_TASK
#define KETTLE_TASK

#include <Arduino.h>
#include <DallasTemperature.h>

extern AppValues appState;
extern DallasTemperature sensors;

void updateKettleTemp(void * parameter){
    for(;;){
        sensors.requestTemperatures();
        appState.kettleTemp = sensors.getTempCByIndex(0);
        vTaskDelay(250 / portTICK_PERIOD_MS);
    }
}

#endif