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
  // Request temperature from the sensor
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);

  // Get sensor address
  DeviceAddress sensorAddress;
  if (sensors.getAddress(sensorAddress, 0)) {
    // Convert sensor address to string and output it
    Serial.print("Sensor Address: ");
    for (uint8_t i = 0; i < 8; i++) {
      if (sensorAddress[i] < 16) Serial.print("0");
      Serial.print(sensorAddress[i], HEX);
    }
    Serial.println();
  } else {
    Serial.println("Error: Could not find the sensor address.");
  }

  // Output the temperature in Celsius
  Serial.print("Temperature (C): ");
  Serial.println(tempC);

  // Add delay to avoid continuous reading
  delay(10000);  // 10 seconds delay
}
