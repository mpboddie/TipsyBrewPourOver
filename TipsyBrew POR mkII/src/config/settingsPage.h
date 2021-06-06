#ifndef SETTINGS_PAGE
#define SETTINGS_PAGE

#include <Arduino.h>

enum SettingType
{
    INT_RANGE,
    ARRAY,
    ON_OFF
};

typedef struct
{
    char* name;
    SettingType type;
    int *value;
    int min;
    int max;
} settingsItem;

#define NUM_OF_SETTINGS 3

settingsItem settingsList[NUM_OF_SETTINGS] = {
    { "Brew Temperature", INT_RANGE, &coffeeSettings.brewTempTarget, 70, 120 },
    { "Preheat Temp", INT_RANGE, &coffeeSettings.preheatTarget, 60, 100 },
    { "Coffee Ratio", INT_RANGE, &coffeeSettings.coffeeRatio, 14, 20 }
};

int currentSetting = 0;

void drawSetting() {
    tft.setTextDatum(MC_DATUM);
    tft.setTextColor(MOSTLY_WHITE, BKGD);
    tft.fillRect(55, 26, 210, 190, BKGD);
    tft.drawString(settingsList[currentSetting].name, 160, 95, 4);
    tft.drawString(String(*settingsList[currentSetting].value), 160, 143, 4);
}

#endif