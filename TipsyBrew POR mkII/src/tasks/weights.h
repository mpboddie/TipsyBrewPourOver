#ifndef WEIGHTS_TASK
#define WEIGHTS_TASK

#include <Arduino.h>
#include <HX711.h>

extern AppValues appState;

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

void initScales() {
    coneScale.begin(CONE_LOADCELL_DOUT_PIN, CONE_LOADCELL_SCK_PIN);
    coneScale.set_scale(cone_calibration_factor);
    //potScale.begin(POT_LOADCELL_DOUT_PIN, POT_LOADCELL_SCK_PIN);
    //potScale.set_scale(pot_calibration_factor);

    if (coneScale.wait_ready_timeout(100)) {
        Serial.println(coneScale.read());
    } else {
        Serial.println(F("[WEIGHTS]Cone scale not found."));
    }

    //tareAll();
}

void updateWeights(void * parameter) {

    for(;;){
        switch (appState.tareMsg) {
            case TARE_CONE:
                Serial.println(F("[WEIGHTS]Tare Cone"));
                tareCone();
                appState.tareMsg = NONE;
                break;
            case TARE_POT:
                Serial.println(F("[WEIGHTS]Tare Pot"));
                tarePot();
                appState.tareMsg = NONE;
                break;
            case TARE_ALL:
                Serial.println(F("[WEIGHTS]Tare All"));
                tareAll();
                appState.tareMsg = NONE;
                break;
            case NONE: {
                Serial.println(F("[WEIGHTS]None"));
                long coneWeight = 0;
                if (coneScale.wait_ready_timeout(100)) {
                    coneWeight = coneScale.read();
                } else {
                    coneWeight = 0;
                }
                //long potWeight = potScale.read();
                if(abs(coneWeight - appState.coneWeight) > 0.2) {
                    Serial.print(F("[WEIGHTS]cone changed: "));
                    Serial.println(appState.coneWeight);
                    appState.weightChange = true;
                    appState.coneWeight = coneWeight;
                }
                /*if(abs(potWeight - appState.potWeight) > 0.2) {
                    appState.weightChange = true;
                    appState.potWeight = potWeight;
                }*/
                break;
            }
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

#endif