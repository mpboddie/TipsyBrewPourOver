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
 
#define LEFT_COL    1
#define RIGHT_COL   2
#define TOP         3
#define BOTTOM      4

#define LITTLE_WIDTH    50
#define BUTTON_HEIGHT   80
#define CORNER_RADIUS   5
#define TOP_START       38
#define BOTTOM_START    120

int littleXcoord(int positionX) {
  if (positionX == LEFT_COL) {
    positionX = 0; 
  } else {
    positionX = tft.width() - LITTLE_WIDTH;
  }
  return positionX;
}

int Ycoord(int positionY) {
  if (positionY == TOP) {
    positionY = TOP_START;
  } else {
    positionY = BOTTOM_START;
  }
  return positionY;
}

int bigXcoord(int positionX) {
  if (positionX == LEFT_COL) {
    positionX = LITTLE_WIDTH - CORNER_RADIUS;
  } else {
    positionX = (tft.width() / 2) + 1;
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
  tft.fillRoundRect(bigXcoord(positionX), Ycoord(positionY), (tft.width()/2) - (LITTLE_WIDTH - CORNER_RADIUS), BUTTON_HEIGHT, CORNER_RADIUS, titleBGcolor);
  tft.setTextColor(titleColor);
  if (caption == "null") {
    tft.drawString(title, bigXcoord(positionX)+10, Ycoord(positionY)+((BUTTON_HEIGHT-tft.fontHeight(4))/2), 4);
  } else {
    int padding = (BUTTON_HEIGHT-tft.fontHeight(4)-tft.fontHeight(2)-5)/2;
    tft.drawString(title, bigXcoord(positionX)+10, Ycoord(positionY)+padding, 4);
    tft.drawString(caption, bigXcoord(positionX)+10, Ycoord(positionY)+padding+tft.fontHeight(4)+5, 2);
  }
  drawLittleButton(positionX, positionY, icon, iconColor, iconBGcolor);
}