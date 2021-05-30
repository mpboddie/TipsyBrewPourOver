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

enum WifiStates
{
  WIFI_DISCONNECTED,
  WIFI_CONNECTING,
  WIFI_CONNECTED
};

struct AppValues {
  AppModeValues currentScreen;
  struct tm timeinfo;
  WifiStates wifiState;
  String ipAddress;
  double kettleTemp;
  bool kettleChange;
  bool screenRefresh;
  String debugMessage;
  bool preheatStatus;
  long activityTimer;
};

#endif