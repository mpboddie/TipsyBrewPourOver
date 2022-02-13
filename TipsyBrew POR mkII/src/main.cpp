#include <Arduino.h>

#include <WiFi.h>
#include <time.h>
//#include <WiFiClient.h>
//#include <WebServer.h>
//#include <ESPmDNS.h>
#include <SPIFFS.h>
//#include <FS.h>
#include <SPI.h>
#include <TFT_eSPI.h> // Graphics and font library for ILI9341 driver chip
#include <OneWire.h>
#include <DallasTemperature.h>
#include <HX711.h>

#include "config/pins.h"
#include "config/userSettings.h"
#include "config/settingsSPIFFS.h"
#include "states.h"

#include "tasks/wifiConnection.h"
#include "tasks/weights.h"
#include "tasks/display.h"
#include "tasks/ntpTime.h"
#include "tasks/kettleTemp.h"

// Which core is Arduino running on
#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

#define BLACK 0x0000
#define WHITE 0xFFFF
#define GREY  0x5AEB

AppValues appState;
TFT_eSPI tft = TFT_eSPI();

OneWire oneWire(ONE_WIRE);
DallasTemperature sensors(&oneWire);

// This is the file name used to store the calibration data
// You can change this to create new calibration files.
// The SPIFFS file name must start with "/".
#define CALIBRATION_FILE "/TouchCalData1"

// Set REPEAT_CAL to true instead of false to run calibration
// again, otherwise it will only be done once.
// Repeat calibration if you change the screen rotation.
#define REPEAT_CAL false

void touch_calibrate();

void setup(void) {
  pinMode( 0, OUTPUT );
  digitalWrite(0, HIGH);

  appState.currentScreen = APP_INTRO_SCREEN;
  appState.kettleTemp = 0;
  appState.kettleChange = true;
  appState.kettleState = false;
  appState.ipAddress = "0.0.0.0";
  appState.wifiState = WIFI_DISCONNECTED;
  appState.debugMessage = "Starting";
  appState.screenRefresh = false;
  appState.preheatStatus = false;
  appState.activityTimer = millis();
  appState.coneWeight = 0.0;
  appState.potWeight = 0.0;
  appState.tareMsg = TARE_ALL;

  // Setup the LCD
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(BKGD);
  tft.setTextColor(TFT_WHITE, BKGD);

  // Calibrate the touch screen and retrieve the scaling factors
  touch_calibrate();

  Serial.begin(115200);
  Serial.println(F("[MAIN]TipsyBrew started"));

  initScales();

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  if(!loadCoffeeSettings(filename, coffeeSettings)) {
    saveCoffeeSettings(filename, coffeeSettings);
  }
  // Uncomment next two lines for debugging settings file issues
  //Serial.print(F("[MAIN]Settings file dump: "));
  //printFile(filename);

  pinMode(PUMP_STEP_PIN, OUTPUT);
	pinMode(PUMP_DIR_PIN, OUTPUT);

  xTaskCreatePinnedToCore(
    keepWiFiAlive,
    "keepWiFiAlive",  // Task name
    5000,             // Stack size (bytes)
    NULL,             // Parameter
    1,                // Task priority
    NULL,             // Task handle
    ARDUINO_RUNNING_CORE
  );
  
  xTaskCreate(
    updateDisplay,
    "UpdateDisplay",  // Task name
    15000,             // Stack size (bytes)
    NULL,             // Parameter
    2,                // Task priority
    NULL              // Task handle
  );
  
  xTaskCreate(
    updateDateTime,
    "UpdateDateTime", // Task name
    5000,             // Stack size (bytes)
    NULL,             // Parameter
    4,                // Task priority
    NULL              // Task handle
  );

  xTaskCreate(
    updateKettleTemp,
    "UpdateKettleTemp", // Task name
    5000,             // Stack size (bytes)
    NULL,             // Parameter
    3,                // Task priority
    NULL              // Task handle
  );

  xTaskCreate(
    updateWeights,
    "UpdateWeights", // Task name
    5000,             // Stack size (bytes)
    NULL,             // Parameter
    3,                // Task priority
    NULL              // Task handle
  );
}

void loop(void) {
  //vTaskDelay(10000 / portTICK_PERIOD_MS);
}

void touch_calibrate()
{
  uint16_t calData[5];
  uint8_t calDataOK = 0;

  // check file system exists
  if (!SPIFFS.begin()) {
    Serial.println("Formating file system");
    SPIFFS.format();
    SPIFFS.begin();
  }

  // check if calibration file exists and size is correct
  if (SPIFFS.exists(CALIBRATION_FILE)) {
    if (REPEAT_CAL)
    {
      // Delete if we want to re-calibrate
      SPIFFS.remove(CALIBRATION_FILE);
    }
    else
    {
      File f = SPIFFS.open(CALIBRATION_FILE, "r");
      if (f) {
        if (f.readBytes((char *)calData, 14) == 14)
          calDataOK = 1;
        f.close();
      }
    }
  }

  if (calDataOK && !REPEAT_CAL) {
    // calibration data valid
    tft.setTouch(calData);
  } else {
    // data not valid so recalibrate
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(20, 0);
    tft.setTextFont(2);
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);

    tft.println("Touch corners as indicated");

    tft.setTextFont(1);
    tft.println();

    if (REPEAT_CAL) {
      tft.setTextColor(TFT_RED, TFT_BLACK);
      tft.println("Set REPEAT_CAL to false to stop this running again!");
    }

    tft.calibrateTouch(calData, TFT_MAGENTA, TFT_BLACK, 15);

    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.println("Calibration complete!");

    // store data
    File f = SPIFFS.open(CALIBRATION_FILE, "w");
    if (f) {
      f.write((const unsigned char *)calData, 14);
      f.close();
    }
  }
}
