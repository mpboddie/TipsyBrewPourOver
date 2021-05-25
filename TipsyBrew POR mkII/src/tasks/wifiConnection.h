#ifndef WIFI_CONNECTION_TASK
#define WIFI_CONNECTION_TASK

#include <Arduino.h>
#include <WiFi.h>

extern AppValues appState;

void keepWiFiAlive(void * parameter){
    for(;;){
        if(WiFi.status() == WL_CONNECTED){
            vTaskDelay(10000 / portTICK_PERIOD_MS);
            continue;
        }

        Serial.println("[WIFI] Connecting");
        appState.wifiState = WIFI_CONNECTING;
        WiFi.mode(WIFI_STA);
        WiFi.setHostname(HOSTNAME);
        WiFi.begin(WIFI_NETWORK, WIFI_PASSWORD);

        unsigned long startAttemptTime = millis();

        // Keep looping while we're not connected and haven't reached the timeout
        while (WiFi.status() != WL_CONNECTED && 
                millis() - startAttemptTime < WIFI_TIMEOUT_MS){
                    appState.wifiState = WIFI_CONNECTING;
                }

        // When we couldn't make a WiFi connection (or the timeout expired)
		  // sleep for a while and then retry.
        if(WiFi.status() != WL_CONNECTED){
            Serial.println("[WIFI] FAILED");
            appState.wifiState = WIFI_DISCONNECTED;
            vTaskDelay(WIFI_RECOVER_TIME_MS / portTICK_PERIOD_MS);
			  continue;
        }

        Serial.print("[WIFI] Connected: ");
        Serial.println(WiFi.localIP());
        appState.wifiState = WIFI_CONNECTED;
        appState.ipAddress = WiFi.localIP().toString();
        //appState.debugMessage = "IP: " + WiFi.localIP().toString() + "    RSSi: " + WiFi.RSSI();
    }
}

#endif