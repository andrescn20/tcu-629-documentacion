//Se importan las librerías necesarias.
#include <WiFi.h>
#include <HTTPClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <time.h>

//Este código debe incluirse una única vez en la placa. Es decir, cuando se desea usar una placa nueva.
//Una vez cargado el código, no es necesario realizar cambios posteriores, aún si se cambian los sensores. 

//Instrucciones:
// 1. Cambiar <Insertar Nombre de Red> por el nombre de red wifi. La red no puede ser 5G.
// 2. Cambiar <Insertar Contraseña> por la contraseña de la red wifi.
// 3. Cambiar <Insertar Identificador> por el identificador de la placa. Debería estar anotado en el dispositivo.

// Credenciales de WiFi
const char* ssid = "<Insertar Nombre de Red>"; //Nombre de red
const char* password = "<Insertar Contraseña>"; //Contraseña de Red

// Conexión con la API (No cambiar)
const char* endpoint = "https://tcu-629-api.azurewebsites.net/SensorsData/Temperature";
const char* subscriptionKey = "c5c9e01b-b707-406e-a99f-9b83f9924b98";

// Definir el identificador de la placa ESP32
const char* boardSerial = "<Insertar Identificador>"; //El identificador es único y debería estar escrito en el dispostivo. 
//En caso de no estarlo, definirlo y escribirlo fisicamente en el dispositivo. 

// Tiempos de espera entre lecturas
const int minutesOnSuccess = 5; //Indica la cantidad de minutos entre mediciones de temperatura
const int minutesOnFailure = 1; //Indica la cantidad de minutos de espera si la medición falla (antes de intentar la siguiente)
const int waitingTimeOnSuccess = minutesOnSuccess * 60000;
const int waitingTimeOnError = minutesOnFailure * 60000; //

// Definir a qué pin de la ESP32 está conectado el bus 1-Wire
#define ONE_WIRE_BUS 5 //PUede variar. Depende de la persona que arma el dispositivo y genera las conexiones.

// Pin del LED (LED integrado)
#define LED_PIN 2

// Crear una nueva instancia de la clase oneWire para comunicarse con cualquier dispositivo OneWire
OneWire oneWire(ONE_WIRE_BUS);

// Pasar la referencia oneWire a la librería DallasTemperature
DallasTemperature sensors(&oneWire);

// Función para conectarse al WiFi
void connectToWiFi() {
  WiFi.begin(ssid, password);
  int retries = 0;
  const int maxRetries = 20;

  // Parpadear el LED mientras se conecta
  while (WiFi.status() != WL_CONNECTED && retries < maxRetries) {
    digitalWrite(LED_PIN, HIGH);
    delay(500);
    digitalWrite(LED_PIN, LOW);
    delay(500);
    retries++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    digitalWrite(LED_PIN, HIGH); // Encender el LED cuando se conecta
  } else {
    // Parpadear más rápido para indicar fallo
    for (int i = 0; i < 10; i++) {
      digitalWrite(LED_PIN, HIGH);
      delay(100);
      digitalWrite(LED_PIN, LOW);
      delay(100);
    }
  }
}

// Función para obtener la hora actual en el formato requerido
String getTimestamp() {
  time_t now;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("No se pudo obtener la hora");
    return "";
  }
  char buf[50];
  strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%S.000Z", &timeinfo);
  return String(buf);
}

void setup() {
  // Iniciar la comunicación serial
  Serial.begin(9600);

  // Configurar el pin del LED como salida
  pinMode(LED_PIN, OUTPUT);

  // Iniciar la librería
  sensors.begin();

  // Conectarse al Wi-Fi
  connectToWiFi();

  // Configurar la hora
  configTime(0, 0, "pool.ntp.org", "time.nist.gov");
}

void loop() {
  // Verificar si aún está conectado al WiFi
  if (WiFi.status() != WL_CONNECTED) {
    connectToWiFi();
  }

  // Enviar el comando para que todos los dispositivos en el bus realicen una conversión de temperatura
  sensors.requestTemperatures();

  // Obtener la temperatura en grados Celsius para el índice del dispositivo
  float tempC = sensors.getTempCByIndex(0);

  // Obtener la dirección del sensor
  DeviceAddress sensorAddress;
  sensors.getAddress(sensorAddress, 0);

  // Convertir la dirección del sensor a texto
  String addressStr = "";
  for (uint8_t i = 0; i < 8; i++) {
    if (sensorAddress[i] < 16) addressStr += "0";
    addressStr += String(sensorAddress[i], HEX);
  }

  // Preparar la información que se envía a la API
  String timestamp = getTimestamp();
  String payload = "{\"boardSerial\": \"" + String(boardSerial) + "\", \"temperature\": " + String(tempC) + ", \"timestamp\": \"" + timestamp + "\", \"sensorAddress\": \"" + addressStr + "\"}";

  // Enviar los datos a la API
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Especificar el destino de la solicitud
    http.begin(endpoint);  // URL de tu API

    // Especificar los encabezados
    http.addHeader("Content-Type", "application/json");
    http.addHeader("Ocp-Apim-Subscription-Key", subscriptionKey);

    // Enviar la solicitud HTTP POST
    int httpResponseCode = http.POST(payload);
    Serial.println(payload);

    // Validar la respuesta
    if (httpResponseCode > 0) {
      // Verificar códigos de éxito
      if (httpResponseCode >= 200 && httpResponseCode < 300) {
        Serial.println("Datos enviados exitosamente");
        String response = http.getString();
        Serial.println(response);
        digitalWrite(LED_PIN, HIGH);  // Mantener el LED encendido para indicar éxito
        delay(1000);
        digitalWrite(LED_PIN, LOW);  // Parpadeo corto para indicar éxito
        delay(waitingTimeOnSuccess);
      } else {
        Serial.print("Código de error: ");
        Serial.println(httpResponseCode);
        String response = http.getString();
        Serial.println(response);
        digitalWrite(LED_PIN, HIGH);  // Parpadear para indicar error
        delay(100);
        digitalWrite(LED_PIN, LOW);
        delay(100);
        digitalWrite(LED_PIN, HIGH);
        delay(100);
        digitalWrite(LED_PIN, LOW);
        delay(waitingTimeOnError);
      }
    } else {
      Serial.print("No se pudo enviar la solicitud. Error: ");
      Serial.println(http.errorToString(httpResponseCode).c_str());
      digitalWrite(LED_PIN, HIGH);  // Parpadear para indicar fallo en la solicitud
      delay(100);
      digitalWrite(LED_PIN, LOW);
      delay(100);
      digitalWrite(LED_PIN, HIGH);
      delay(100);
      digitalWrite(LED_PIN, LOW);
      delay(waitingTimeOnError);
    }

    // Liberar recursos
    http.end();
  }
}
