#ifndef PINS
#define PINS

// One-wire bus (used by temp sensor)
#define ONE_WIRE                    22

// Weight sensors
#define CONE_LOADCELL_DOUT_PIN  5       // Cone HX711 DOUT pin
#define CONE_LOADCELL_SCK_PIN   17      // Cone HX711 SCK pin

#define POT_LOADCELL_DOUT_PIN   16      // Pot HX711 DOUT pin
#define POT_LOADCELL_SCK_PIN    4       // Pot HX711 SCK pin

// Steppers
#define PUMP_DIR_PIN            34
#define PUMP_STEP_PIN           35
#define PUMP_STEPS_REV          200
#define PUMP_INTERFACE_TYPE     1

#endif