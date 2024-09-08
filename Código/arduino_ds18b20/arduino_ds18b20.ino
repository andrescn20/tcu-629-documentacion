// Incluir las librerías de Arduino requeridas para comunicarse con el sensor:
#include "OneWire.h"
#include "DallasTemperature.h"

// Definir a qué pin del Arduino está conectado el bus 1-Wire:
#define ONE_WIRE_BUS 5

// Crear una nueva instancia de la clase oneWire para comunicarse con cualquier dispositivo OneWire:
OneWire oneWire(ONE_WIRE_BUS);

// Pasar la referencia oneWire a la librería DallasTemperature:
DallasTemperature sensors(&oneWire);

void setup() {
  // Iniciar la comunicación serial a una velocidad de 9600 baudios:
  Serial.begin(9600);
  // Iniciar la librería:
  sensors.begin();
}

void loop() {
  // Enviar el comando para que todos los dispositivos en el bus realicen una conversión de temperatura:
  sensors.requestTemperatures();

  // Obtener la temperatura en grados Celsius para el índice del dispositivo:
  float tempC = sensors.getTempCByIndex(0); // el índice 0 se refiere al primer dispositivo

  // Imprimir la temperatura en Celsius en el Monitor Serial:
  Serial.print("Temperatura: ");
  Serial.print(tempC);
  Serial.print(" \xC2\xB0"); // muestra el símbolo de grado
  Serial.print("C  |  ");

  //Esperar 1 segundo.
  //Se puede cambiar este valor para modificar la frecuencia de muestreo. 
  delay(1000);
}
