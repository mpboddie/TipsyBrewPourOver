#ifndef WEIGHTS_TASK
#define WEIGHTS_TASK

#include <Arduino.h>
#include <HX711.h>

extern AppValues appState;
//extern HX711 coneScale;
//extern HX711 potScale;

HX711 coneScale;
HX711 potScale;

void tareCone() {
    coneScale.tare();
    appState.weightChange = true;
}

void tarePot() {
    potScale.tare();
    appState.weightChange = true;
}

void tareAll() {
    tareCone();
    tarePot();
    appState.weightChange = true;
}

void updateWeights(void * parameter) {

    coneScale.begin(CONE_LOADCELL_DOUT_PIN, CONE_LOADCELL_SCK_PIN);
    coneScale.set_scale(cone_calibration_factor);
    //tareCone();

    potScale.begin(POT_LOADCELL_DOUT_PIN, POT_LOADCELL_SCK_PIN);
    potScale.set_scale(pot_calibration_factor);
    //tarePot();

    tareAll();

    for(;;){
        switch (appState.tareMsg) {
            case TARE_CONE:
                tareCone();
                appState.tareMsg = NONE;
                break;
            case TARE_POT:
                tarePot();
                appState.tareMsg = NONE;
                break;
            case TARE_ALL:
                tareAll();
                appState.tareMsg = NONE;
                break;
            case NONE: {
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
                break;
            }
        }
        vTaskDelay(250 / portTICK_PERIOD_MS);
    }
}

#endif