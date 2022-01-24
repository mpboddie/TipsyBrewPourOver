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

enum TareMsg
{
  TARE_CONE,
  TARE_POT,
  TARE_ALL,
  NONE
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
  long coneWeight;
  long potWeight;
  bool weightChange;
  TareMsg tareMsg;
};

#endif