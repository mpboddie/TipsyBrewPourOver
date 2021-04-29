#ifndef STATES
#define STATES

enum AppModeValues
{
  APP_INTRO_SCREEN,
  APP_HOME,
  APP_SETTINGS,
  APP_LOGS,
  APP_COFFEE
};

struct AppValues {
  AppModeValues currentScreen;
  String time;
  int8_t wifiStrength;
  double kettleTemp;
};

#endif