#ifndef COFFEE_PAGE
#define COFFEE_PAGE

#include <Arduino.h>

enum CoffeePage {
    PREPARE,
    WEIGH,
    BREWING
};

CoffeePage currCoffeePage = PREPARE;


void initCoffee() {
    switch (currCoffeePage) {
        case PREPARE:
            tft.setTextColor(MOSTLY_WHITE, BKGD);
            tft.setTextDatum(TL_DATUM);
            tft.drawString("PREPARE", 55, 65, 4);
            tft.drawString("1. Put Carafe in TipsyBrew", 55, 65+5+tft.fontHeight(4), 2);
            tft.drawString("2. Put EMPTY Cone in TipsyBrew", 55, 65+(2*5)+tft.fontHeight(2)+tft.fontHeight(4), 2);
            tft.drawString("3. Put Filter in Cone", 55, 65+(3*5)+(2*tft.fontHeight(2))+tft.fontHeight(4), 2);
            
            drawLittleButton(LEFT_OFF, TOP, rain, TFT_BLUE, MOSTLY_WHITE);
            drawLittleButton(LEFT_OFF, BOTTOM, home, TFT_BLACK, MOSTLY_WHITE);
            drawLittleButton(RIGHT_OFF, TOP, cupStraw, TFT_BLACK, GREY_GRAY);
            drawLittleButton(RIGHT_OFF, BOTTOM, cup, MOSTLY_WHITE, TB_ORANGE);
            break;
        case WEIGH:
            tft.setTextColor(MOSTLY_WHITE, BKGD);
            tft.setTextDatum(TC_DATUM);
            tft.drawString("SELECT RECIPE:", tft.width()/2, 30, 2);
            tft.drawString("GROUNDS (g)", (tft.width()/2-55)/2+55, tft.height()/2, 2);
            tft.drawString("WATER (mL)", (tft.width()/2-55)/2+tft.width()/2, tft.height()/2, 2);
            tft.drawString("COFFEE (mL)", tft.width()/2, tft.height()-25-tft.fontHeight(4)-5-tft.fontHeight(2), 2);
            drawLittleButton(LEFT_OFF, TOP, arrowLeft, TFT_BLUE, MOSTLY_WHITE);
            drawLittleButton(LEFT_OFF, BOTTOM, arrowReturnLeft, TFT_BLACK, MOSTLY_WHITE);
            drawLittleButton(RIGHT_OFF, TOP, arrowRight, TFT_BLUE, MOSTLY_WHITE);
            drawLittleButton(RIGHT_OFF, BOTTOM, coffeeBean, MOSTLY_WHITE, TB_ORANGE);

            // sample data for layout purposes
            /*tft.drawString("29.8", (tft.width()/2-55)/2+55, tft.height()/2+5+tft.fontHeight(2), 4);
            tft.drawString("447.0", (tft.width()/2-55)/2+tft.width()/2, tft.height()/2+5+tft.fontHeight(2), 4);
            tft.drawString("387.5", tft.width()/2, tft.height()-25-tft.fontHeight(4), 4);*/
            tft.setTextDatum(MC_DATUM);
            tft.drawString("Default", tft.width()/2, tft.height()/2-(BUTTON_HEIGHT/2), 4);
            break;
        case BREWING:
            tft.setTextColor(MOSTLY_WHITE, BKGD);
            break;
    }
}

void coffeeBeat() {
    switch (currCoffeePage) {
        case PREPARE:
            // Nothing to do here
            break;
        case WEIGH: {
            // Update weights
            if(appState.weightChange) {
                appState.weightChange = false;
                tft.fillRect(55, tft.height()/2+5+tft.fontHeight(2), tft.width()-110, tft.fontHeight(4), BKGD);
                tft.fillRect(55, tft.height()-25-tft.fontHeight(4), tft.width()-110, tft.fontHeight(4), BKGD);
                tft.setTextDatum(TC_DATUM);
                char str[7];
                dtostrf(appState.coneWeight, 5, 1, str);
                tft.drawString(str, (tft.width()/2-55)/2+55, tft.height()/2+5+tft.fontHeight(2), 4);                // Cone weight AKA weight of the coffee grounds
                dtostrf(appState.coneWeight * coffeeSettings.coffeeRatio, 5, 1, str);
                tft.drawString(str, (tft.width()/2-55)/2+tft.width()/2, tft.height()/2+5+tft.fontHeight(2), 4);     // Grounds weight * coffee ratio = amount of water used for this brew
                dtostrf(appState.coneWeight * (coffeeSettings.coffeeRatio - 1.995), 5, 1, str);
                tft.drawString(str, tft.width()/2, tft.height()-25-tft.fontHeight(4), 4);          
            }
            break;
        }
    }
}

void coffeeTouch(int x, int y) {
    switch (currCoffeePage) {
        case PREPARE:
            if(x < 55 && y > 25 && y < tft.height()/2) {
                // Top Left (Rinse Filter)
                // TODO: Make a filter rinse function you dim witted twat
            } else if(x < 55 && y > tft.height()/2 && y < tft.height()-25) {
                // Bottom Left (Home)
                appState.currentScreen = APP_HOME;
                appState.screenRefresh = true;
            } else if (x > 265 && y > 25 && y < tft.height()/2) {
                // Top Right (Iced Coffee)
                // TODO: This is a special case of the normal brew, implement it AFTER a normal brew is done
            } else if (x > 265 && y > tft.height()/2 && y < tft.height()-25) {
                // Bottom Right (Continue with Brew)
                currCoffeePage = WEIGH;
                appState.screenRefresh = true;
                appState.tareMsg = TARE_ALL;
            };
            break;
        case WEIGH:
            // This screen doesn't exist yet, so pay no attention to my copy pasta here
            if(x < 55 && y > 25 && y < tft.height()/2) {
                // Top Left (Rinse Filter)
                // TODO: Make a filter rinse function you dim witted twat
            } else if(x < 55 && y > tft.height()/2 && y < tft.height()-25) {
                // Bottom Left (Home)
                currCoffeePage = PREPARE;
                appState.screenRefresh = true;
            } else if (x > 265 && y > 25 && y < tft.height()/2) {
                // Top Right (Iced Coffee)
                // TODO: This is a special case of the normal brew, implement it AFTER a normal brew is done
            } else if (x > 265 && y > tft.height()/2 && y < tft.height()-25) {
                // Bottom Right (Continue with Brew)
                currCoffeePage = BREWING;
                appState.screenRefresh = true;
            };
            break;
    }
}

#endif