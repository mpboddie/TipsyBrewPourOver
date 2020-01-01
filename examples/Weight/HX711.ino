/*
 Sample code made for the LCD Keypad Arduino Shield
 12/31/2019 Added some HX711 code, check out https://learn.sparkfun.com/tutorials/load-cell-amplifier-hx711-breakout-hookup-guide/introduction for more info
*/

// assigning int for each button
#define button_none 0
#define button_select 1
#define button_left 2
#define button_down 3
#define button_up 4
#define button_right 5

// include the library code:
#include <LiquidCrystal.h>
#include "HX711.h"

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to

// Pins used by the shield
const int rs = 8, en = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7;

int keypad_in;
int keypad_old = 9999;
bool button_pressed = false;

//This value is obtained using the SparkFun_HX711_Calibration sketch
#define calibration_factor 1695.0

#define LOADCELL_DOUT_PIN  50
#define LOADCELL_SCK_PIN  51

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
HX711 scale;

// Quiick utility function for clearing just a single line of the LCD
void clearLine(int lineNo) {
  lcd.setCursor(0, lineNo);
  lcd.print("                ");
  lcd.setCursor(0, lineNo);
}

char lcdMsg[100];

// Smoothing weight readings
const int numReadings = 10;

float weightReadings[numReadings];      // the readings from the weight sensor
int readIndex = 0;                      // the index of the current reading
float total = 0.0;                      // the running total
float average = 0.0;                    // the average

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("TipsyBrew");
  lcd.setCursor(0, 1);
  lcd.print("and so it begins");

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(calibration_factor);
  //Assuming there is no weight on the scale at start up, reset the scale to 0
  scale.tare();
  // initialize all the weights to 0:
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    weightReadings[thisReading] = 0.0;
  }
}

void loop() {
  
  keypad_in = analogRead(0);
  // The value of keypad_in can jump around a point, so I put in a 10 point buffer
  button_pressed = (abs(keypad_in - keypad_old) > 10) ? true : false;
  
  // My approximate values, I assume it may vary
  // No buttons pressed = 1023
  // Select = 725
  // Left = 483
  // Up = 131
  // Down = 308
  // Right = 0
  
  if (button_pressed) {
    keypad_old = keypad_in;
    //clearLine(1);
    if (keypad_in > 875) {
      //lcd.print("None");
    } else if (keypad_in > 600) {
      //lcd.print("Select Pressed");
      scale.tare();
      // initialize all the weights to 0:
      for (int thisReading = 0; thisReading < numReadings; thisReading++) {
        weightReadings[thisReading] = 0.0;
      }
      readIndex = 0;
      total = 0.0;
      average = 0.0;
      clearLine(1);
      lcd.print("Scale tared!");
    } else if (keypad_in > 400) {
      //lcd.print("Left Pressed");
    } else if (keypad_in > 200) {
      //lcd.print("Down Pressed");
    } else if (keypad_in > 75) {
      //lcd.print("Up Pressed");
    } else {
      //lcd.print("Right Pressed");
    }
  }

  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  
  // subtract the last reading:
  total = total - weightReadings[readIndex];
  // read from the sensor:
  weightReadings[readIndex] = scale.get_units();
  // add the reading to the total:
  total = total + weightReadings[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;

  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }

  // calculate the average:
  average = total / numReadings;
  // send it to the computer as ASCII digits
  clearLine(1);
  lcd.print("Weight: ");
  lcd.print(average);
  lcd.print("g");
  delay(10);        // delay in between reads for stability
}
