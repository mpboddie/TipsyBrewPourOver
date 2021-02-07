/*  ################################
 *  #   Hardware pin definitions   #
 *  ################################
 *  An Arduino MEGA 2560 is being used. If you are trying to use different hardware make sure you make the appropriate changes here
 */

// ### Weight sensor ###
#define LOADCELL_DOUT_PIN  11   // HX711 DOUT pin
#define LOADCELL_SCK_PIN  12    // HX711 SCK pin
HX711 scale;

// ### Screen definitions ###

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ### Keypad definitions ###

const byte ROWS = 2; 
const byte COLS = 3; 

char keys[ROWS][COLS] = {
  {'C','L','U'},
  {'D','S','R'}
};
byte rowPins[ROWS] = {7, 6}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {3, 4, 5}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

// ### Pump definitions ###

// Define pin connections
#define enA 8
#define in1 9
#define in2 10

// ### Experimental spout movement ###
#define anglePin 22
#define distancePin 23

ServoEasing angleServo;
ServoEasing distanceServo;

// ### Kettle kontrol ###
#define kettlePin 24

ServoEasing kettleServo;

// This is the logo used at start up... It is beautiful!!!
const unsigned char TipsyBrew [] PROGMEM = {
  // 'tipsybrew', 128x64px
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x03, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x0f, 0xff, 0xc1, 0xff, 0x73, 0xfe, 0x3f, 0xce, 0x1d, 0xfe, 0x3f, 0xe7, 0xfb, 0x8e, 0x38, 
0x00, 0x3f, 0xff, 0xc1, 0xff, 0x7b, 0xff, 0x7f, 0xce, 0x3d, 0xff, 0x3f, 0xf7, 0xfb, 0xce, 0x38, 
0x00, 0x7f, 0xff, 0x01, 0xff, 0x7b, 0xff, 0x7f, 0xee, 0x3d, 0xff, 0x3f, 0xf7, 0xfb, 0xce, 0x78, 
0x00, 0xff, 0xf8, 0x78, 0x38, 0x7b, 0xcf, 0x71, 0xef, 0x39, 0xc7, 0x38, 0xf7, 0x81, 0xce, 0x78, 
0x01, 0xff, 0xe1, 0xf8, 0x38, 0x7b, 0xcf, 0x71, 0xe7, 0x39, 0xc7, 0x3c, 0xf7, 0x81, 0xcf, 0x70, 
0x03, 0xff, 0xc7, 0xf8, 0x38, 0x7b, 0xcf, 0x71, 0xe7, 0x79, 0xc7, 0x3c, 0xf7, 0x81, 0xdf, 0x70, 
0x07, 0xff, 0x9f, 0xfc, 0x38, 0x7b, 0xcf, 0x78, 0xc7, 0x71, 0xc7, 0x3c, 0xf7, 0x81, 0xdf, 0x70, 
0x0f, 0xff, 0x3f, 0xfc, 0x38, 0x7b, 0xcf, 0x3c, 0x03, 0xf1, 0xc7, 0x3c, 0xf7, 0x81, 0xdf, 0x70, 
0x0f, 0xff, 0x3f, 0xf8, 0x38, 0x7b, 0xcf, 0x1e, 0x03, 0xf1, 0xff, 0x3b, 0xf7, 0xf9, 0xdf, 0x70, 
0x1f, 0xfe, 0x7f, 0xf8, 0x38, 0x7b, 0xcf, 0x0f, 0x03, 0xe1, 0xfe, 0x3b, 0xf7, 0xf9, 0xff, 0x70, 
0x1f, 0xfe, 0xff, 0xf8, 0x38, 0x7b, 0xff, 0x07, 0x81, 0xe1, 0xff, 0x3b, 0xe7, 0xf8, 0xfb, 0xf0, 
0x1f, 0xfc, 0xff, 0xf0, 0x38, 0x7b, 0xff, 0x03, 0xc1, 0xe1, 0xc7, 0xbb, 0x87, 0x80, 0xfb, 0xe0, 
0x1f, 0xfc, 0xff, 0xf0, 0x38, 0x7b, 0xc4, 0x01, 0xe1, 0xc1, 0xc7, 0xbb, 0xc7, 0x80, 0xfb, 0xe0, 
0x1f, 0xf9, 0xff, 0xe0, 0x38, 0x7b, 0xc0, 0x79, 0xe1, 0xc1, 0xc7, 0xb9, 0xc7, 0x80, 0xfb, 0xe0, 
0x1f, 0xf1, 0xff, 0xc0, 0x38, 0x7b, 0xc0, 0x71, 0xe1, 0xc1, 0xc7, 0xbd, 0xe7, 0x80, 0xfb, 0xe0, 
0x0f, 0xe3, 0xff, 0x80, 0x38, 0x7b, 0xc0, 0x71, 0xe1, 0xc1, 0xc7, 0xbc, 0xe7, 0x80, 0xf9, 0xe0, 
0x0f, 0x8f, 0xff, 0x00, 0x38, 0x7b, 0xc0, 0x7f, 0xe1, 0xc1, 0xff, 0xbc, 0xe7, 0xf8, 0x71, 0xe0, 
0x00, 0x3f, 0xfe, 0x00, 0x38, 0x7b, 0xc0, 0x7f, 0xe1, 0xc1, 0xff, 0xbc, 0xf7, 0xfc, 0x71, 0xe0, 
0x01, 0xff, 0xfc, 0x00, 0x38, 0x7b, 0xc0, 0x3f, 0xc1, 0xc1, 0xff, 0x38, 0x77, 0xfc, 0x71, 0xc0, 
0x01, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x7f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};