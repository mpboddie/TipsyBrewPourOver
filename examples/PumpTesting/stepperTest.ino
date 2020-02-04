// Pump Test
// This is a program to help test and evaluate stepper driven peristaltic pumps
// Up/Down adjusts test speed while motor is not running
// Left/Right will run pump for set period of time forwards/backwards
// Select will stop pump if running or tare scale if pump isn't running

#include <LiquidCrystal.h>
#include "LcdKeypad.h"
#include "HX711.h"

// Define pin connections
const int dirPin = 2;   // Stepper direction pin
const int stepPin = 3;  // Stepper step pin

#define LOADCELL_DOUT_PIN  11   // HX711 DOUT pin
#define LOADCELL_SCK_PIN  12    // HX711 SCK pin

// Calibration factor dependent on your load cell
#define calibration_factor 1695.0

byte btn;
char strbuf[LCD_COLS + 1]; // one line of lcd display

int stepDelay = 2000;

float weightRead;

unsigned long currentMillis = millis();
unsigned long startMillis;
const unsigned long weightDuration = 100;
const unsigned long duration = 5000;
unsigned long weightMillis = currentMillis - weightDuration;

enum AppModeValues
{
  APP_NORMAL_MODE,
  APP_MOTOR_MOVING
};

byte appMode = APP_NORMAL_MODE;

const int pumpForward = HIGH;
const int pumpReverse = LOW;

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

HX711 scale;

void clearLine(int lineNo, int colNo=0, int width=16);
void printWeightValue(bool force=false);
void tareScale();
void setDirection(int direction);
void runPump(int speed);

void setup()
{
  // Declare pins as Outputs
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(calibration_factor);
  //Assuming there is no weight on the scale at start up, reset the scale to 0
  scale.tare();
  
  backLightOn();
  // set up the LCD's number of columns and rows:
  lcd.begin(LCD_COLS, LCD_ROWS);
  setBacklightBrightness(0);

  lcd.print("TipsyBrew");
  lcd.setCursor(0, 1);
  lcd.print("'Fuck you Tipsy'");
  delay(5000);
  clearLine(1);
  clearLine(0);
  lcd.print("Weight: ");
  lcd.setCursor(0, 1);
  lcd.print("Speed: ");
  lcd.print(stepDelay);
}

void loop()
{
  btn = getButton();

  switch (appMode)
  {
    case APP_NORMAL_MODE :
      if (btn == BUTTON_SELECT_PRESSED)
      {
        // tare the scale
        tareScale();
      } else if (btn == BUTTON_UP_PRESSED) {
        // increase speed
        stepDelay -= 100;
        clearLine(1, 7, 9);
        lcd.print(stepDelay);
      } else if (btn == BUTTON_DOWN_PRESSED) {
        // decrease speed
        stepDelay += 100;
        clearLine(1, 7, 9);
        lcd.print(stepDelay);
      } else if (btn == BUTTON_LEFT_PRESSED) {
        // set direction to counter clockwise and change to the run pump mode
        setDirection(pumpReverse);
        appMode = APP_MOTOR_MOVING;
      } else if (btn == BUTTON_RIGHT_PRESSED) {
        // set direction to clockwise and change to the run pump mode
        setDirection(pumpForward);
        appMode = APP_MOTOR_MOVING;
      }
      break;
    case APP_MOTOR_MOVING :
      if (btn == BUTTON_SELECT_PRESSED) {
        // stop the motor and go back to normal mode
        appMode = APP_NORMAL_MODE;
      } else {
        // motor stuff
        runPump(stepDelay);
      }
      break;
  }
  printWeightValue();
}

void clearLine(int lineNo, int colNo, int width) {
  lcd.setCursor(colNo, lineNo);
  for (int x = 0; x < width; x++) {
    lcd.print(" ");
  }
  lcd.setCursor(colNo, lineNo);
}

void printWeightValue(bool force) {
  
  currentMillis = millis();
  
  // get the weight value
  weightRead = scale.get_units();
  
  // refreshing the display too quickly makes it difficult to see
  if (force || (currentMillis - weightMillis >= weightDuration)) {
    
    // clear the line you want to display it on
    clearLine(0, 8, 7);
    
    // print the weight properly formatted
    lcd.print(weightRead, 1);
    lcd.print(" g");

    // reset the weight display counter
    weightMillis = currentMillis;
  }
}

void tareScale() {
  // tare the scale
  scale.tare();
  printWeightValue(true);
}

void setDirection(int direction) {
  digitalWrite(dirPin, direction);
}

void runPump(int speed) {
  digitalWrite(stepPin, HIGH);
  delayMicroseconds(speed);
  digitalWrite(stepPin, LOW);
  delayMicroseconds(speed);
}
