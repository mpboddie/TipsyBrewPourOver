// THIS IS A SAMPLE USER SETTINGS FILE
// Copy the contents to a userSettings.h file of your own and customize the values.

// WiFi settings
// These must be customized for your network. It should be self explanitory.
// TODO: add this to a file on the SD card or make some sort of setup process
const char *ssid     = <<<INSERT_YOUR_SSID>>>;
const char *password = <<<INSERT_YOUR_WIFI_PASSWORD>>>;

// BLYNK SETTINGS
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = <<<INSERT_YOUR_BLYNK_TOKEN>>>;

// NTP SETTINGS
char ntpServer[] = "us.pool.ntp.org";
int ntpTimeOffset = -18000;   // This sets the timezone, New York is the default value here
unsigned long ntpUpdateInterval = 60000;

// LOAD CELL SETTINGS
// This value is obtained using the SparkFun_HX711_Calibration sketch
#define pot_calibration_factor 2020.0
#define cone_calibration_factor 2120.0

// APPEARANCE SETTINGS
int tftBrightness = 30;
char background[] = "backgrounds/hexagonDark.bmp";
char startUpLogo[] = "logo.bmp";

// CUSTOM COLORS
#define TB_ORANGE       0xFA20    // 255, 68, 0
#define MOSTLY_WHITE    0xAD75    // 175, 175, 175
#define BKGD            0x0000    // 0, 0, 0
#define GREY_GRAY       0x8410    // 128, 128, 128
