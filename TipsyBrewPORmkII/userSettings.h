
// WiFi settings
// These must be customized for your network. It should be self explanitory.
// TODO: add this to a file on the SD card or make some sort of setup process
const char *ssid     = "EyesOnBack";
const char *password = "Il!kech33z!ts";

// BLYNK SETTINGS
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "KzX-26OxNkX5b_EjkEAHgAwhZYgZGHfd";

// NTP SETTINGS
char ntpServer[] = "us.pool.ntp.org";
int ntpTimeOffset = -18000;
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

// HEATER SETTINGS
// These should be moved to the settings menu once it exists
float preHeatTarget = 80.0;
float triggerLow = 5.0;
float triggerHigh = 1.0;
unsigned long preHeatDuration = 900000;
float brewTarget = 97.0;

// KETTLE SETTINGS
// These are the servo positions for the kettle switch
// They may need to be calibrated due to changes in the servo, arm installation, or kettle type
#define KETTLE_ON                   74
#define KETTLE_NEUTRAL              50
#define KETTLE_OFF                  32
