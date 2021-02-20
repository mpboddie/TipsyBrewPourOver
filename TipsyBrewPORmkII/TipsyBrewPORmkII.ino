#include <WiFiEspAT.h>            // communication to the ESP with AT commands
#include <BlynkSimpleWifi.h>      // blynk mobile apps
#include "HX711.h"                // weight sensors
#include <arduino-timer.h>        // asynchronous timers
#include <TFT_ILI9341.h>          // https://github.com/Bodmer/TFT_ILI9341
#include <NTPClient.h>            // Accessing the series of tube to look at a sun dial
#include <WiFiUdp.h>              // NTP uses UDP, but not ABC or XYZ
#include <OneWire.h>              // Used by the following...
#include <DallasTemperature.h>    // Temp sensors are on OneWire bus
#include "ServoEasing.h"          // Servo for kettle

#include "userSettings.h"        // User configurable settings
#include "sprites.h"
#include "appStates.h"

// PIN SETTINGS

// Pot load cell
#define POT_LOADCELL_DOUT_PIN       3
#define POT_LOADCELL_SCK_PIN        2
// Cone load cell
#define CONE_LOADCELL_DOUT_PIN      5
#define CONE_LOADCELL_SCK_PIN       4
// Pump driver
#define enA                         22
#define in1                         23
#define in2                         24
// Display and SD card
#define SD_CS                       7      // SD card select pin 
#define TFT_CS                      8      // TFT CS  pin is connected to arduino pin 8
#define TFT_RST                     9      // TFT RST pin is connected to arduino pin 9
#define TFT_DC                      10     // TFT DC  pin is connected to arduino pin 10
#define TFT_LED                     13
// UI Buttons
#define LEFT_UP                     18
#define LEFT_DOWN                   19
#define RIGHT_UP                    20
#define RIGHT_DOWN                  21
// One-wire bus (used by temp sensor)
#define ONE_WIRE                    11
// Servo pins
#define KETTLE_SWITCH_PIN           25
           
// WiFi setup
#define AT_BAUD_RATE 115200
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, ntpServer, ntpTimeOffset, ntpUpdateInterval);
unsigned long rawTime; 
unsigned long ntpHours;
unsigned long ntpMinutes;
String topBarTime;

// initialize ILI9341 TFT library
TFT_ILI9341 tft = TFT_ILI9341();

// initialize Load Cells
HX711 potScale;
HX711 coneScale;
float potWeight;
float coneWeight;
float blynkPotW = 0;
float blynkConeW = 0;

// initialize timers
auto preHeatTimer = timer_create_default();

// initialize temp sensor
OneWire oneWire(ONE_WIRE);
DallasTemperature sensors(&oneWire);
float kettleTemp = 0;
float kettleTempBuff = 0;
unsigned long kettleOnEvent = 0;
unsigned long kettleOffEvent = millis();

// Kettle switch
Servo kettleServo;
bool preHeatStatus = false;

#define NOT_READY       1
#define IN_PROCESS      2
#define READY_NOW       3
WidgetLED notReady(V7);
WidgetLED inProcess(V8);
WidgetLED readyNow(V9);
void brewerStateLED(int state);

#include "uiElements.h"
#include "commonFunctions.h"
#include "headerFooter.h"
#include "loadingScreen.h"
#include "homeScreen.h"

void setup() {
  Serial.begin(115200);
  while (!Serial);

  // TFT and SD init
  pinMode(TFT_LED, OUTPUT);
  tft.init();

  tft.fillScreen(TFT_BLACK);
  analogWrite(TFT_LED, tftBrightness);
  tft.setRotation(1);

  initLoadingScreen();
  //int cursorX = 10;
  //int cursorY = 238;  
  
  // CONNECT TO WIFI
  // TODO: make sure things work even when Wifi is not present (great coffee existed before Wifi)
  Serial3.begin(AT_BAUD_RATE);
  WiFi.init(Serial3);

  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    wifiIconOff();
    loadingText("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  // waiting for connection to Wifi network set with the SetupWiFiConnection sketch
  Serial.println("Waiting for connection to WiFi");
  loadingText("Waiting for connection to WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print('.');
  }
  Serial.println("You're connected to the network");
  wifiIconOn();
  loadingText("You're connected to the network");

  // Blynk init
  Blynk.config(auth, BLYNK_DEFAULT_DOMAIN, BLYNK_DEFAULT_PORT);
  while(Blynk.connect() != true) {}
  loadingText("Blynk connected");

  // NTP init
  timeClient.begin();
  loadingText("NTP connected");
  timeClient.update();

  // Pot Load Cell init
  potScale.begin(POT_LOADCELL_DOUT_PIN, POT_LOADCELL_SCK_PIN);
  potScale.set_scale(pot_calibration_factor);
  potScale.tare();
  // Cone Load Cell init
  coneScale.begin(CONE_LOADCELL_DOUT_PIN, CONE_LOADCELL_SCK_PIN);
  coneScale.set_scale(cone_calibration_factor);
  coneScale.tare(); 

  // INIT FRONT PANEL BUTTONS
  pinMode(LEFT_UP, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(LEFT_UP), LeftUpper, RISING);
  pinMode(LEFT_DOWN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(LEFT_DOWN), LeftLower, RISING);
  pinMode(RIGHT_UP, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(RIGHT_UP), RightUpper, RISING);
  pinMode(RIGHT_DOWN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(RIGHT_DOWN), RightLower, RISING);

  // Pump init
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);

  // Temp sensor init
  sensors.begin();
  sensors.setResolution(9);
  
  printTime();
  clearLoadingText();
  fadeOutLoading();
  switchToHome();
}

BLYNK_WRITE(V1)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable

  // process received value
  if(pinValue == 1) {
    coneWeight = 0;
    coneScale.tare();
    potWeight = 0;
    potScale.tare();
    Blynk.virtualWrite(V5, coneWeight);
    Blynk.virtualWrite(V6, potWeight);
  }
}

void LeftUpper() {
  // LEFT_UP button action
  switch (appMode)
  {
    case APP_INTRO_SCREEN :
      // do nothing
      break;
    case APP_HOME :
      homeLeftUpper();
      break;
  }
}

void LeftLower() {
  // LEFT_DOWN button action
  switch (appMode)
  {
    case APP_INTRO_SCREEN :
      // do nothing
      break;
    case APP_HOME :
      homeLeftLower();
      break;
  }
}

void RightUpper() {
  switch (appMode)
  {
    case APP_INTRO_SCREEN :
      // do nothing
      break;
    case APP_HOME :
      coneScale.tare();
      break;
  }
}

void RightLower() {
  switch (appMode)
  {
    case APP_INTRO_SCREEN :
      // do nothing
      break;
    case APP_HOME :
      potScale.tare();
      break;
  }
}

void brewerStateLED(int state) {
  switch (state)
  {
    case NOT_READY :
      notReady.on();
      inProcess.off();
      readyNow.off();
      break;
    case IN_PROCESS :
      notReady.off();
      inProcess.on();
      readyNow.off();
      break;
    case READY_NOW :
      notReady.off();
      inProcess.off();
      readyNow.on();
      break;
  }
}

void loop() {
  // Once in loop no blocking code should be run. Your goal for loop is once you enter, do what is needed as fast as possible then get right back to the start of loop.
  // TODO: Add link to TipsyBrew post explaining the above in more detail.
  //analogWrite(TFT_LED, tftBrightness);
  Blynk.run();
  timeClient.update();
  if (!topBarTime.equals(getShortFormattedTime())) {
    printTime();
  }
  preHeatTimer.tick();
  sensors.requestTemperatures();
  kettleTempBuff = kettleTemp;
  kettleTemp = sensors.getTempCByIndex(0);
  switch (appMode)
  {
    case APP_INTRO_SCREEN :
      // do nothing
      break;
    case APP_HOME :
      // temporarily displaying the scale values for now
      coneWeight = coneScale.get_units();
      potWeight = potScale.get_units();
      if (abs(coneWeight - blynkConeW) >= 0.25) {
        Blynk.virtualWrite(V5, coneWeight);
        blynkConeW = coneWeight;
      }
      if (abs(potWeight - blynkPotW) >= 0.25) {
        Blynk.virtualWrite(V6, potWeight);
        blynkPotW = potWeight;
      }
      if (kettleTempBuff != kettleTemp) {
        kettleTempBuff = kettleTemp;
        footerPrintTemp();
      }
      break;
  }
  if (preHeatStatus) {
    // check boiler temp, turn it on if it needs heat
    if (kettleTemp <= (preHeatTarget - triggerLow)) {             // Kettle temp is low, We PROBABLY want to turn on
      if (!isKettleOn()) {
        footerPrintMsg("Kettle On");
        kettleOn();                 // Kettle was off, we definitely want to turn it on
      } else if ((millis() - kettleOnEvent) > 300000) {
        // Kettle was on for more than 5 minutes (we may want to adjust this time) and not up to temp yet, this could indicate that there is a problem
        // Problems such as...
        //    temp probe is disconnected
        //    temp probe is not in or near the heated water
        //    there is insufficient water in the kettle
        // These could cause safety or equipment issues, so we should throw an error and halt
        cancelPreHeat();
        footerPrintMsg("PREHEAT ISSUE", true);
      }
    } else if (kettleTemp >= (preHeatTarget + triggerHigh)) {     // Kettle temp is high, We DO want to turn off
      if (isKettleOn()) {
        footerPrintMsg("Kettle Off");
        kettleOff();                // Kettle was on, turn it off
      } else if (kettleTemp > kettleTempBuff && (millis() - kettleOffEvent > 300000)) {
        // Kettle was turned off more than 5 minutes ago (might need to adjust time) and temp is still increasing, this could indicate a faliure to turn off the kettle
        cancelPreHeat();            // If there is ever a failure to turn off the kettle, preheat should not be used
        footerPrintMsg("PREHEAT OVERRUN", true);
      }
    } else {                                                      // We MIGHT do something
      // I might add some logic here, I'm going to test what I have first
    }
  }
}
