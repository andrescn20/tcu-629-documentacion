/* DS18B20 1-Wire digital temperature sensor with Arduino example code. More info: https://www.makerguides.com */

// Include the required Arduino libraries:
#include "OneWire.h"
#include "DallasTemperature.h"

// Define to which pin of the Arduino the 1-Wire bus is connected:
#define ONE_WIRE_BUS 5

// Create a new instance of the oneWire class to communicate with any OneWire device:
OneWire oneWire(ONE_WIRE_BUS);

// Pass the oneWire reference to DallasTemperature library:
DallasTemperature sensors(&oneWire);


void setup() {
  // Begin serial communication at a baud rate of 9600:
  Serial.begin(9600);
  // Start up the library:
  sensors.begin();
}

void loop() {
  // Send the command for all devices on the bus to perform a temperature conversion:
  sensors.requestTemperatures();

  // Fetch the temperature in degrees Celsius for device index:
  float tempC = sensors.getTempCByIndex(0); // the index 0 refers to the first device

  // Print the temperature in Celsius in the Serial Monitor:
  Serial.println(tempC);

  // Wait 1 second:
  delay(1000);
}