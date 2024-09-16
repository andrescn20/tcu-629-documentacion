#include <OneWire.h>
#include <DallasTemperature.h>

// Pin for the 1-Wire bus
#define ONE_WIRE_BUS 5

// Initialize OneWire and DallasTemperature libraries
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(9600);
  sensors.begin();
}

void loop() {
  // Get sensor address (unique identifier)
  DeviceAddress sensorAddress;
  if (sensors.getAddress(sensorAddress, 0)) {
    // Print the unique identifier (ROM code) of the sensor
    Serial.print("Unique Identifier (ROM Code): ");
    for (uint8_t i = 0; i < 8; i++) {
      if (sensorAddress[i] < 16) Serial.print("0");
      Serial.print(sensorAddress[i], HEX);
    }
    Serial.println();
  } else {
    Serial.println("Error: Could not find the sensor address.");
  }

  // Add delay to avoid continuous reading
  delay(5000);  // 10 seconds delay
}
