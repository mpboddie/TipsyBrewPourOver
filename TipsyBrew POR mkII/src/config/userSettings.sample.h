#ifndef USER_SETTINGS
#define USER_SETTINGS

// WiFi settings
// These must be customized for your network. It should be self explanitory.
#define WIFI_NETWORK            ENTER_YOUR_SSID
#define WIFI_PASSWORD           ENTER_YOUR_PASSWORD
#define WIFI_TIMEOUT_MS         20000 // 20 second WiFi connection timeout
#define WIFI_RECOVER_TIME_MS    30000 // Wait 30 seconds after a failed connection attempt

#define HOSTNAME                "TipsyBrew"

// NTP settings
const char* ntpServer =             "us.pool.ntp.org";
const long  gmtOffset_sec =         -18000;     // US East Coast
const int   daylightOffset_sec =    3600;

// CUSTOM COLORS
// These colors are in the RGB565 format
#define TB_ORANGE       0xFA20    // 255, 68, 0
#define MOSTLY_WHITE    0xD69A    // 210, 210, 210
#define BKGD            0x0000    // 0, 0, 0
#define GREY_GRAY       0x8410    // 128, 128, 128

const char *titleSlogan =       "POUR OVER ROBOT";
const char *tbVersion =         "Mark II v0.1";

#define DEBOUNCE_MS     250

#endif