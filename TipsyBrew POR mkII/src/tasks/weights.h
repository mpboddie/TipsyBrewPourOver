#ifndef WEIGHTS_TASK
#define WEIGHTS_TASK

#include <Arduino.h>
#include <HX711.h>

extern AppValues appState;
//extern HX711 coneScale;
//extern HX711 potScale;

HX711 coneScale;
HX711 potScale;

void updateWeights(void * parameter) {

    coneScale.begin(CONE_LOADCELL_DOUT_PIN, CONE_LOADCELL_SCK_PIN);
    coneScale.set_scale(cone_calibration_factor);
    coneScale.tare();

    potScale.begin(POT_LOADCELL_DOUT_PIN, POT_LOADCELL_SCK_PIN);
    potScale.set_scale(pot_calibration_factor);
    potScale.tare();

    for(;;){
        float coneWeight = coneScale.get_units();
        float potWeight = potScale.get_units();
        if(abs(coneWeight - appState.coneWeight) > 0.2) {
            appState.weightChange = true;
            appState.coneWeight = coneWeight;
        }
        if(abs(potWeight - appState.potWeight) > 0.2) {
            appState.weightChange = true;
            appState.potWeight = potWeight;
        }
        vTaskDelay(250 / portTICK_PERIOD_MS);
    }
}

#endif