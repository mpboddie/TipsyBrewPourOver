/*******************
 * Buttons
 * 
 * There are two types of buttons, Big and Little.
 * All buttons are either TOP_LEFT, BOTTOM_LEFT, TOP_RIGHT, BOTTOM_RIGHT
 * 
 * Big buttons span half the screen width and consist of:
 *    locationX - LEFT_COL or RIGHT_COL
 *    locationY - TOP or BOTTOM
 *    icon - 40x40 icon from sprites
 *    icon color - color to draw said icon
 *    icon background color - background color for icon portion of button 
 *    title - String to label the button
 *    title text color - Color of the title text
 *    title background color - background color for the title portion of the button
 *    title caption - Optional short description for button
 *    
 * Little buttons are just 50px wide and only consist of the icon items and location from the Big buttons.
 */

#ifndef DRAW_BUTTONS
#define DRAW_BUTTONS

#define LEFT_ON     1
#define RIGHT_ON    2
#define LEFT_OFF    3
#define RIGHT_OFF   4
#define TOP         5
#define BOTTOM      6

#define LITTLE_WIDTH          50
#define BUTTON_HEIGHT         80
#define CORNER_RADIUS         5
#define BUTTON_TOP_PADDING    2  
#define BUTTON_SIDE_PADDING   2

#include <Arduino.h>
#include <TFT_eSPI.h> // Graphics and font library for ILI9341 driver chip

extern TFT_eSPI tft;

int littleXcoord(int positionX) {
  switch(positionX) {
    case LEFT_OFF :
      positionX = 0;
      break;
    case RIGHT_OFF :
      positionX = tft.width() - LITTLE_WIDTH;
      break;
    case LEFT_ON :
      positionX = (tft.width() / 2) - BUTTON_SIDE_PADDING - LITTLE_WIDTH;
      break;
    case RIGHT_ON :
      positionX = (tft.width() / 2) + BUTTON_SIDE_PADDING;
      break;
  }
  return positionX;
}

int Ycoord(int positionY) {
  if (positionY == TOP) {
    positionY = (tft.height() / 2) - BUTTON_HEIGHT - BUTTON_TOP_PADDING;
  } else {
    positionY = (tft.height() / 2) + BUTTON_TOP_PADDING;
  }
  return positionY;
}

int bigXcoord(int positionX) {
  switch (positionX) {
    case LEFT_OFF :
    case LEFT_ON :
      positionX = 0;
      break;
    case RIGHT_OFF :
    case RIGHT_ON :
      positionX = (tft.width() / 2) + BUTTON_SIDE_PADDING;
      break; 
  }
  return positionX;
}

void drawLittleButton(  int positionX, 
                        int positionY, 
                        const uint8_t *icon, 
                        uint16_t iconColor, 
                        uint16_t iconBGcolor) {
  positionX = littleXcoord(positionX);
  positionY = Ycoord(positionY);
  tft.fillRoundRect(positionX, positionY, LITTLE_WIDTH, BUTTON_HEIGHT, CORNER_RADIUS, iconBGcolor);
  tft.drawBitmap(positionX + 5, positionY + 20, icon, 40, 40, iconColor);
}

void drawBigButton( int positionX, 
                    int positionY, 
                    const uint8_t *icon, 
                    uint16_t iconColor, 
                    uint16_t iconBGcolor,
                    const char *title,
                    uint16_t titleColor,
                    uint16_t titleBGcolor,
                    const char *caption = "null") {
  tft.fillRoundRect(bigXcoord(positionX), Ycoord(positionY), (tft.width() / 2) - BUTTON_SIDE_PADDING, BUTTON_HEIGHT, CORNER_RADIUS, titleBGcolor);
  tft.setTextColor(titleColor);
  int titleBuffer = 0;
  if (positionX == LEFT_OFF || positionX == RIGHT_ON) { titleBuffer = LITTLE_WIDTH; }
  if (caption == "null") {
    tft.drawString(title, bigXcoord(positionX)+titleBuffer+10, Ycoord(positionY)+((BUTTON_HEIGHT-tft.fontHeight(4))/2), 4);
  } else {
    int padding = (BUTTON_HEIGHT-tft.fontHeight(4)-tft.fontHeight(2)-5)/2;
    tft.drawString(title, bigXcoord(positionX)+titleBuffer+10, Ycoord(positionY)+padding, 4);
    tft.drawString(caption, bigXcoord(positionX)+titleBuffer+10, Ycoord(positionY)+padding+tft.fontHeight(4)+5, 2);
  }
  drawLittleButton(positionX, positionY, icon, iconColor, iconBGcolor);
}

#endif