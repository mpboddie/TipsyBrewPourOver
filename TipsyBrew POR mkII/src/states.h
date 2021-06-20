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
  float kettleTemp;
  bool kettleChange;
  bool kettleState;
  bool screenRefresh;
  String debugMessage;
  bool preheatStatus;
  long activityTimer;
  long safetyTimer;
  float coneWeight;
  float potWeight;
  bool weightChange;
};

#endif