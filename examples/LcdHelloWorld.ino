/*
 Sample code made for the LCD Keypad Arduino Shield
*/

// include the library code:
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to

// Pins used by the shield
const int rs = 8, en = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7;

int keypad_in;
int keypad_old = 9999;
bool button_pressed = false;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Quiick utility function for clearing just a single line of the LCD
void clearLine(int lineNo) {
  lcd.setCursor(0, lineNo);
  lcd.print("                ");
  lcd.setCursor(0, lineNo);
}

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("TipsyBrew");
  lcd.setCursor(0, 1);
  lcd.print("and so it begins");
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);

  keypad_in = analogRead(0);
  button_pressed = (keypad_in != keypad_old) ? true : false;
  
  // My approximate values, I assume it may vary
  // No buttons pressed = 1023
  // Select = 725
  // Left = 483
  // Up = 131
  // Down = 308
  // Right = 0
  
  if (button_pressed) {
    keypad_old = keypad_in;
    clearLine(1);
    lcd.print("A0: ");
    lcd.print(keypad_in);
  }
}
