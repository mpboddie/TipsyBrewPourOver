#ifndef SETTINGS_SPIFFS
#define SETTINGS_SPIFFS

#include <Arduino.h>
#include <SPIFFS.h>
#include <ArduinoJSON.h>

struct CoffeeSettings {
  int preheatTarget;
  int brewTempTarget;
  int coffeeRatio;
};

const char *filename = "/settings.json"; 
CoffeeSettings coffeeSettings;

// Loads the configuration from a file
void loadCoffeeSettings(const char *filename, CoffeeSettings &coffeeSettings) {

  File file = SPIFFS.open(filename, "r");

  // Allocate a temporary JsonDocument
  // Don't forget to change the capacity to match your requirements.
  // Use https://arduinojson.org/v6/assistant to compute the capacity.
  StaticJsonDocument<512> doc;

  // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc, file);
  if (error)
    Serial.println(F("Failed to read file, using default configuration"));

  // Copy values from the JsonDocument to the Config
  coffeeSettings.preheatTarget = doc["preheat"] | 80;
  //strlcpy(config.hostname,                  // <- destination
  //        doc["hostname"] | "example.com",  // <- source
  //        sizeof(config.hostname));         // <- destination's capacity
  coffeeSettings.brewTempTarget = doc["brewTemp"] | 100;
  coffeeSettings.coffeeRatio = doc["ratio"] | 18;

  file.close();
}

// Saves the configuration to a file
void saveCoffeeSettings(const char *filename, const CoffeeSettings &coffeeSettings) {
  // Delete existing file, otherwise the configuration is appended to the file
  SPIFFS.remove(filename);

  // Open file for writing
  File file = SPIFFS.open(filename, "w");
  if (!file) {
    Serial.println(F("Failed to create file"));
    return;
  }

  // Allocate a temporary JsonDocument
  // Don't forget to change the capacity to match your requirements.
  // Use https://arduinojson.org/assistant to compute the capacity.
  StaticJsonDocument<256> doc;

  // Set the values in the document
  doc["preheat"] = coffeeSettings.preheatTarget;
  doc["brewTemp"] = coffeeSettings.brewTempTarget;
  doc["ratio"] = coffeeSettings.coffeeRatio;

  // Serialize JSON to file
  if (serializeJson(doc, file) == 0) {
    Serial.println(F("Failed to write to file"));
  }

  file.close();
}

// Prints the content of a file to the Serial
void printFile(const char *filename) {
  // Open file for reading
  File file = SPIFFS.open(filename, "r");
  if (!file) {
    Serial.println(F("Failed to read file"));
    return;
  }

  // Extract each characters by one by one
  while (file.available()) {
    Serial.print((char)file.read());
  }
  Serial.println();

  // Close the file
  file.close();
}

#endif