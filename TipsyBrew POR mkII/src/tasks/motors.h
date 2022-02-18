#ifndef MOTORS_TASK
#define MOTORS_TASK

#include <Arduino.h>
#include <AccelStepper.h>

extern AppValues appState;
extern AccelStepper pumpMotor;

void updateMotors(void * parameter){
    for(;;){
        
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

#endif