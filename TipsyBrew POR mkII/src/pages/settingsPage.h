#ifndef SETTINGS_PAGE
#define SETTINGS_PAGE

#include <Arduino.h>

enum SettingType
{
    INT_RANGE,
    ARRAY,
    ON_OFF
};

// TODO: Right now settings are just integers, this needs to change
typedef struct
{
    char* name;
    SettingType type;
    int *value;
    int min;
    int max;
    char* info;
} settingsItem;

#define NUM_OF_SETTINGS 3

settingsItem settingsList[NUM_OF_SETTINGS] = {
    { "Brew Temperature", INT_RANGE, &coffeeSettings.brewTempTarget, 70, 120, "Target kettle temperature for \nbrewing delicious coffee" },
    { "Preheat Temp", INT_RANGE, &coffeeSettings.preheatTarget, 60, 100, "The preheat function will aim for\nthis temp. It is best to set this \nlower than the brew temp." },
    { "Coffee Ratio", INT_RANGE, &coffeeSettings.coffeeRatio, 14, 20, "For each gram of coffee \ngrounds, TipsyBrew will use this many mL of water. Lower =\nStronger" }
};

int currentSetting = 0;

void drawSetting() {
    tft.setTextDatum(MC_DATUM);
    tft.setTextColor(MOSTLY_WHITE, BKGD);
    tft.fillRect(55, 26, tft.width()-(55*2), tft.height()-(25*2), BKGD);
    tft.drawString(settingsList[currentSetting].name, tft.width()/2, 65, 4);
    tft.drawString(String(*settingsList[currentSetting].value), tft.width()/2, tft.fontHeight(4)+70, 4);
    tft.setViewport(55, tft.fontHeight(4)*2+75, tft.width()-(55*2), tft.fontHeight(2)*4);
    tft.setTextDatum(TL_DATUM);
    tft.setCursor(0, 0, 2);
    tft.println(settingsList[currentSetting].info);
    tft.resetViewport();
}

#endif