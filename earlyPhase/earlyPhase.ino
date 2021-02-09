// TipsyBrew Pour Over Machine
// by Tipsy
// See http://tipsybrew.com for complete project details
// Version 0.5
// This is a rough start connecting the following components in unison
// - Load cell with HX711 amplifier
// - Diaphragm pump (12v) with L298N driver
// - SSD1306 OLED display (128x64)
// - Keypad 2x3
// Initially this is going to have a limited feature set. You may find it pretty
// rigid and mostly hard coded, however I've had a lot of odd fluke like setbacks
// so I just want to get this thing making a cup of coffee.

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Keypad.h>
#include "HX711.h"
#include "ServoEasing.h"
#include "triggy.h"
#include "earlyLibs.h"
#include "screens.h"

// ### App settings ###
// Variables that are required or encouraged to be customized will go here

const byte coffeeRatio = 16;  // Coffee to water ratio (lower number = stronger coffee)
const float waterLoss = 1.995;  // This is used to calculate the amount of water is retained by the coffee grounds

// Calibration factor dependent on your load cell
// You absolutely need to tweak this for your hardware
// See https://tipsybrew.com/blog/2020-01-01-the-hookup-weight-sensor/ for more info
// If you have problems with this let me know so I can make a better calibration guide
#define calibration_factor 1695.0

// Pump speeds
// These define the speed of the pump during bloom and brew, higher = faster
#define BLOOM_SPEED 100
#define BREW_SPEED 90
#define BLOOM_WAIT 30000
#define DRAWDOWN_TIME 20000

// Pour arm settings
// These values should be changed to fit your brewer. If you have a limited space there is nothing preventing the servo from turning into your stand, so maybe test gently.

// left to right is referred to as the X
//const int leftAngle = 88;   // servo setting to set arm to the human's far left perspective
//const int rightAngle = 124; // servo setting to set arm to the human's far right perspective
#define MID_ANGLE 94  // This should be 90 on a 180 degree servo... but sometimes shit happens and the worlds don't align

// front to back is referred to as the Y
const int frontDistance = 5; // servo setting to set arm to the human's far front perspective
const int backDistance = 180; // servo setting to set arm to the human's far back perspective

// number of ms between steps of arm move
// NOTE: this is kind of done in a dumb way, but it is an example of GEFN (good enough for now)
#define ARM_SPEED 100

// Do change the pourCoord arrays if you want
#define NUM_BLOOM_SPOTS 10
pourCoord bloomSpots[NUM_BLOOM_SPOTS];

// BREW_RADIUS and BREW_DISTANCE are in servo steps as units
// Radius defines the max radius of the brew circle
// Distance is the servo arm distance to the center of the brewer, assuming the arm is also angle center
#define BREW_RADIUS 60
#define BREW_DISTANCE 102.5
#define BREW_RESOLUTION 20  // must divide 360 evenly
pourCoord brewSpots[BREW_RESOLUTION];

// Kettle values
#define KETTLE_ON 74
#define KETTLE_NEUTRAL 50
#define KETTLE_OFF 32

// Turns on a servo test during the startup screen
const bool servoTest = false;
// debug flag
const bool debug = false;

// Cone limit
// I STRONGLY SUGGEST you start small and work your way up if you so choose on cone limit.
// This limits the number of ml dispensed during a brew cycle before pausing for a drawdown.
// Let me say it another way, if you increase this too much HOT WATER WILL OVERFLOW making a huge mess in the best case scenario.
const int coneLimit = 225;

// ### Functions and variable initialization ###

enum AppModeValues
{
  APP_INTRO_SCREEN,
  APP_NORMAL_MODE,
  APP_SERVO_TEST,
  APP_FILTER_RINSE,
  APP_CONFIRM_START,
  APP_BLOOM,
  APP_BREW,
  APP_BREW_COMPLETE
};

byte appMode = APP_INTRO_SCREEN;

void runPump();
void stopPump();
void pumpSpeed(byte dcSpeed);

// Timing variables
unsigned long startMillis;
unsigned long currentMillis;

// Weight variables
float weightRead = 0;
float prevWeight = 0;
float groundWeight = 0;

// Bloom time
unsigned long bloomTimer;

// Brew variables
int brewPosition = 0;
int currentAngle;
int currentDistance;

void setup() {
  Serial.begin(9600);

  // initialize with the I2C addr 0x3C
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Set all the motor control pins to outputs
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);

  // Turn off motors - Initial state
  stopPump();
  
  // Title screen... completely unecessary
  display.clearDisplay();
  // Display bitmap
  display.drawBitmap(0, 0, TipsyBrew, 128, 64, WHITE);
  display.display();
  startMillis = millis();
  while( millis() - startMillis < 5000 ) {
    if ( keypad.getKey() ) break;
  }

  // populate brew path
  float angleIncrement = 360/BREW_RESOLUTION;
  for (int i=0; i<BREW_RESOLUTION; i++) {
    Serial.print(i);
    double distance = SASdistance(BREW_DISTANCE, BREW_RADIUS, i*angleIncrement);
    double angle = SSAngle(BREW_RADIUS, i*angleIncrement, distance) + MID_ANGLE;
    //distance = reverseDistance(distance);
    brewSpots[i] = {(int)angle, (int)distance};
    
    Serial.print(F(" - ("));
    Serial.print(angle, 1);
    Serial.print(F(", "));
    Serial.print(distance, 1);
    Serial.println(F(")"));
    if (debug) {
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0,10);
      display.println(i);
      display.print(angle, 1);
      display.print(F(", "));  
      display.println(distance, 1);
      display.display();
      delay(500);
    }
  }

  int bloomRadius = 30;
  angleIncrement = 360/NUM_BLOOM_SPOTS;
  for (int i=0; i<NUM_BLOOM_SPOTS; i++) {
    double distance = SASdistance(BREW_DISTANCE, bloomRadius, i*angleIncrement);
    double angle = SSAngle(bloomRadius, i*angleIncrement, distance) + MID_ANGLE;
    distance = reverseDistance(distance);
    bloomSpots[i] = {(int)angle, (int)distance};
  }
  
  if ( servoTest ) {
    Serial.println(F("Servo Test"));
    // servo test cycle
    angleIncrement = 360/BREW_RESOLUTION;
    angleServo.attach(anglePin, 1300, 1700);
    distanceServo.attach(distancePin);
    angleServo.setSpeed(10);
    distanceServo.setSpeed(70);
    distanceServo.write(93);
    angleServo.write(93);
    delay(3000);
    for (int i=0; i < BREW_RESOLUTION; i++) {
      Serial.print(i*angleIncrement);
      Serial.print(F(" - ("));
      Serial.print(brewSpots[i].angle, 1);
      Serial.print(F(", "));
      Serial.print(brewSpots[i].distance, 1);
      Serial.println(F(")"));
      distanceServo.startEaseTo(brewSpots[i].distance);
      angleServo.startEaseTo(brewSpots[i].angle);
      synchronizeAllServosAndStartInterrupt();
      while(distanceServo.isMoving() || angleServo.isMoving()) {
        if ( distanceServo.isMoving() ) {
          Serial.print(F("Distance "));
        }
        if ( angleServo.isMoving() ) {
          Serial.print(F("Angle"));
        }
        Serial.println(F(" "));
      }
    }
    distanceServo.startEaseTo(brewSpots[0].distance);
    angleServo.startEaseTo(brewSpots[0].angle);
    synchronizeAllServosAndStartInterrupt();
    while(distanceServo.isMoving() || angleServo.isMoving()) {
      if ( distanceServo.isMoving() ) {
        Serial.print(F("Distance "));
      }
      if ( angleServo.isMoving() ) {
        Serial.print(F("Angle"));
      }
      Serial.println(F(" "));
    }
    angleServo.detach();
    distanceServo.detach();
    Serial.print(F("Done with Servo Test"));
  }

  startKettle();
  
  // Make sure the pour spout is out of the way for convenient dead bean loading
  angleServo.attach(anglePin, 1000, 2000);
  distanceServo.attach(distancePin);
  angleServo.write(MID_ANGLE);
  distanceServo.write(frontDistance);
  delay(500);
  distanceServo.detach();
  angleServo.detach();
  
  // Intro
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,10);
  display.println(F("Place a prepared"));
  display.println(F("brewer in the machine"));  
  display.println(F("WITHOUT any grounds"));
  display.display();
  delay(1000);
  display.setCursor(10,44);
  display.println(F("Press any key to"));
  display.println(F("continue."));
  display.display();

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(calibration_factor);
  //Assuming there is no weight on the scale at start up, reset the scale to 0
  scale.tare();
}

void loop() {
  
  char key = keypad.getKey(); // Read the key

  switch (appMode)
  {
    case APP_INTRO_SCREEN :
      if (key) {
        if (key == 'U') {
          display.clearDisplay();
          appMode = APP_FILTER_RINSE;
        } else if (key == 'L') {
          display.clearDisplay();
          currentAngle = 90;
          currentDistance = 90;
          angleServo.attach(anglePin, 1000, 2000);
          distanceServo.attach(distancePin);
          angleServo.write(currentAngle);
          distanceServo.write(currentDistance);
          delay(100);
          angleServo.detach();
          distanceServo.detach();
          appMode = APP_SERVO_TEST;
        } else {
          scale.tare();
          display.clearDisplay();
          appMode = APP_NORMAL_MODE;
          weightRead = -100; // this forces a refresh
        }
      }
      break;
    case APP_SERVO_TEST :
      displayServoTest(currentAngle, currentDistance);
      if (key == 'U') {
        if (currentDistance < 180) {
          currentDistance++;
          distanceServo.attach(distancePin);
          distanceServo.write(currentDistance);
          delay(100);
          distanceServo.detach();
        }
      } else if (key == 'D') {
        if (currentDistance > 0) {
          currentDistance--;
          distanceServo.attach(distancePin);
          distanceServo.write(currentDistance);
          delay(100);
          distanceServo.detach();
        }
      } else if (key == 'L') {
        if (currentAngle > 0) {
          currentAngle--;
          angleServo.attach(anglePin, 1000, 2000);
          angleServo.write(currentAngle);
          delay(100);
          angleServo.detach();
        }
      } else if (key == 'R') {
        if (currentAngle < 180) {
          currentAngle++;
          angleServo.attach(anglePin, 1000, 2000);
          angleServo.write(currentAngle);
          delay(100);
          angleServo.detach();
        }
      } else if (key) {
        display.clearDisplay();
        appMode = APP_INTRO_SCREEN;
        displayIntro();
      }
      break;
    case APP_FILTER_RINSE :
      displayFilterRinse();
      if (key == 'L') {
        display.clearDisplay();
        appMode = APP_NORMAL_MODE;
      } else if (key == 'D') {
        display.clearDisplay();
        display.setTextSize(2);
        display.setTextColor(WHITE);
        display.setCursor(0,10);
        display.println(F("  Rinsing"));
        display.println(F("    In"));
        display.println(F(" Progress"));
        display.display();
        int filterResolution = 36;
        pourCoord filterRinse[filterResolution];
        drawCircle(50, filterResolution, BREW_DISTANCE, MID_ANGLE, filterRinse);
        angleServo.attach(anglePin, 1000, 2000);
        distanceServo.attach(distancePin);
        pumpSpeed(BREW_SPEED);
        runPump();
        for(int i = 0; i < filterResolution; i++) {
          angleServo.write(filterRinse[i].angle);
          distanceServo.write(filterRinse[i].distance);
          delay(200);
        }
        angleServo.write(filterRinse[0].angle);
        distanceServo.write(filterRinse[0].distance);
        delay(100);
        stopPump();
        angleServo.detach();
        distanceServo.detach();
      }
      break;
    case APP_NORMAL_MODE :
      // Display active weight, calculate what the weight means for finished coffee
      prevWeight = weightRead;
      weightRead = scale.get_units();
      if ( weightRead < 1 ) {
        display.clearDisplay();
        display.setTextSize(2);
        display.setTextColor(WHITE);
        display.setCursor(0,20);
        display.println(F(" FEED ME"));
        display.println(F("DEAD BEANS"));
        display.display();
      } else if ( weightRead < 5 ) {
        display.clearDisplay();
        display.setTextSize(2);
        display.setTextColor(WHITE);
        display.setCursor(0,10);
        display.println(F("MORE BEANS"));
        display.println(F(" STOOPID"));
        display.println(F("HOOMAN!!!"));
        display.display();
      } else if ( abs(weightRead - prevWeight) > 0.1 ) { // refresh display
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.setCursor(0,10);
        display.print(F("Dead beans: "));
        display.print(weightRead, 1);
        display.println(F(" g"));
        display.print(F("Water used: "));
        display.print((weightRead*coffeeRatio), 1);
        display.println(F(" ml"));
        display.print(F("Coffee made:"));
        display.print(((weightRead*coffeeRatio) - (weightRead*waterLoss)), 1);
        display.println(F(" ml")); 
        display.println(F(" "));
        display.println(F("Press UP to tare"));
        display.println(F("COFFEE NOW to Brew"));
        display.display();  
      }
      if ( key == 'U' ) {
        // Tare the scale with the A button
        scale.tare();
      } else if ( key == 'C' ) {
        // Start the brew with the * button
        appMode = APP_CONFIRM_START;
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.setCursor(0,0);
        display.println(F("You are about to make"));
        display.print(((weightRead*coffeeRatio) - (weightRead*waterLoss)), 1);
        display.println(F(" ml of coffee"));
        display.println(F(" "));
        display.println(F("Are you sure?"));
        display.println(F(" "));
        display.println(F("SELECT to Continue"));
        display.println(F("ANY KEY to Cancel"));
        display.display();
      }
      break;
    case APP_CONFIRM_START :
      // Confirm start with the 1 button
      if ( key == 'S' ) {
        groundWeight = weightRead;
        appMode = APP_BLOOM;
        display.clearDisplay();
      } else if ( key ) {
        appMode = APP_NORMAL_MODE;
        display.clearDisplay();
        weightRead = -100; // forces a refresh
      }
      break;
    case APP_BLOOM :
      // Dispense total water*bloom and wait
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0,0);
      display.println(F("Bloom in Progress!"));
      display.println(F("Do not touch brewer"));
      display.display();
      scale.tare(); // weight from this point on is water dispensed only

      // To start the bloom put the spout dead center
      angleServo.attach(anglePin, 1000, 2000);
      distanceServo.attach(distancePin);
      angleServo.write(MID_ANGLE);
      distanceServo.write(BREW_DISTANCE);
      delay(500);
      distanceServo.detach();
      angleServo.detach();
      
      pumpSpeed(BLOOM_SPEED);
      runPump();
      bloomTimer = millis();
      while (scale.get_units() < groundWeight) {
        // possibly display weight to the user, but do they really care?
        // just do nothing for now
      }
      bloomTimer = millis() - bloomTimer; // tells us how long the first half of the bloom took
      stopPump();

      brewPosition = 0;
      bloomTimer = 0;
      angleServo.attach(anglePin, 1000, 2000);
      distanceServo.attach(distancePin);
      runPump();
      while (scale.get_units() < (2*groundWeight)) {
        if ( millis() >= bloomTimer + ARM_SPEED ) {
          bloomTimer = millis();
          angleServo.write(bloomSpots[brewPosition % NUM_BLOOM_SPOTS].angle);
          distanceServo.write(bloomSpots[brewPosition % NUM_BLOOM_SPOTS].distance);
          brewPosition += 1;
        }
      }
      stopPump();
      distanceServo.detach();
      angleServo.detach();
      
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0,20);
      display.println(F("     BLOOMING"));
      display.println(F("Do not touch brewer"));
      display.print(scale.get_units(), 1);
      display.println(F(" g dispensed"));
      display.display();
      delay(BLOOM_WAIT);

      // Adjust spout to center
      angleServo.attach(anglePin, 1000, 2000);
      distanceServo.attach(distancePin);
      currentAngle = (int) MID_ANGLE;
      currentDistance = (int) BREW_DISTANCE;
      angleServo.write(currentAngle);
      distanceServo.write(currentDistance);
      delay(100);
      distanceServo.detach();
      angleServo.detach();
      
      // Move on to the brewing step
      appMode = APP_BREW;
      break;
    case APP_BREW :
      pumpSpeed(BREW_SPEED);
      bool complete = false;
      float waterRound = scale.get_units();   // This acts as a sort of tare without losing live total weight, subtract it from the current weight to get the weight of the current pour
      
      // For loop dispense <coneLimit> ml of water or amount remaining then wait
      while (scale.get_units() < (coffeeRatio * groundWeight)) { // stay in loop until we have pumped the needed water
        float waterLeft = (coffeeRatio * groundWeight) - scale.get_units();   // live count of water still required
        if ( waterLeft > coneLimit ) {
          waterLeft = coneLimit;    // if the amount of water required is over the amount of water you want in the cone at one moment just pour the max amount
        } else {
          complete = true;          // else pour what is left and call it
        }
        
        startMillis = millis();
        brewPosition = 0;
        unsigned long lastMove = startMillis;
        
        angleServo.attach(anglePin, 1000, 2000);
        distanceServo.attach(distancePin);
        
        while ( waterLeft > (scale.get_units() - waterRound) ) {
          startMillis = millis();
          display.clearDisplay();
          display.setTextSize(1);
          display.setTextColor(WHITE);
          display.setCursor(0,0);
          display.println(F("Brewing Cycle!!!"));
          display.println(F("Do not touch the brewer"));
          display.print(scale.get_units(), 1);
          display.print(F(" of "));
          display.print(coffeeRatio * groundWeight, 1);
          display.println(F("g"));
          
          if ( complete ) {
            display.println(F("Last Round!"));
          }
          display.display();
          runPump();
          
          if (lastMove + ARM_SPEED <= startMillis) {
            angleServo.write(brewSpots[brewPosition % BREW_RESOLUTION].angle);
            distanceServo.write(brewSpots[brewPosition % BREW_RESOLUTION].distance);
            lastMove = startMillis;
            brewPosition++;
          }
        }
        stopPump();
        
        if ( complete ) {
          appMode = APP_BREW_COMPLETE;
          angleServo.detach();
          distanceServo.detach();
        } else {
          display.clearDisplay();
          display.setTextSize(1);
          display.setTextColor(WHITE);
          display.setCursor(0,0);
          display.println(F("Drawdown Cycle!!!"));
          display.println(F("Do not touch brewer"));
          display.print(scale.get_units(), 1);
          display.println(" g dispensed");
          display.display();
          currentAngle = (int) MID_ANGLE;
          currentDistance = (int) BREW_DISTANCE;
          angleServo.write(currentAngle);
          distanceServo.write(currentDistance);
          delay(1000);
          angleServo.detach();
          distanceServo.detach();
          delay(DRAWDOWN_TIME - 1000);
          waterRound = scale.get_units();
        }
      }
      appMode = APP_BREW_COMPLETE;
      break;
    case APP_BREW_COMPLETE :
      // Display that brew is done and wait for input
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0,0);
      display.println(F("Brewing is complete"));
      display.println(F("Allow time for drawdown"));
      display.println(F("if still neccessary"));
      display.println(F(" "));
      display.println(F("Press any key to"));
      display.println(F("continue"));
      display.display();
      if ( key ) {
        appMode = APP_NORMAL_MODE;
        display.clearDisplay();
      }
      break;
  }
}

void runPump() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
}

void stopPump() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
}

void pumpSpeed(byte dcSpeed) {
  analogWrite(enA, dcSpeed);
}

void startKettle() {
  kettleServo.attach(kettlePin);
  kettleServo.write(KETTLE_ON);
  delay(1000);
  kettleServo.write(KETTLE_NEUTRAL);
  delay(1000);
  kettleServo.detach();
}

void stopKettle() {
  kettleServo.attach(kettlePin);
  kettleServo.write(KETTLE_OFF);
  delay(1000);
  kettleServo.write(KETTLE_NEUTRAL);
  delay(1000);
  kettleServo.detach();
}
