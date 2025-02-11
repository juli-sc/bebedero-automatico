#include <WiFi.h>
#include <Callmebot_ESP32.h>

const char* ssid = "Sebastian";
const char* password = "salchipapa1";
String phoneNumber = "+573052748311";
String apiKey = "1716418";

const int buttonPin = 4;  // Pin al que está conectado el botón

int buttonState = HIGH;     // Variable para almacenar el estado actual del botón
int lastButtonState = HIGH; // Variable para almacenar el último estado del botón
const int debounceDelay = 50;  // Tiempo de debounce en milisegundos
unsigned long lastDebounceTime = 0;  // Tiempo de la última lectura estable del botón

void setup() {
  Serial.begin(115200);

  // Configuración del botón
  pinMode(buttonPin, INPUT_PULLUP);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  int reading = digitalRead(buttonPin);

  // Si el estado del botón ha cambiado
  if (reading != lastButtonState) {
    lastDebounceTime = millis();  // Reinicia el contador de debounce
  }

  // Si ha pasado suficiente tiempo desde el último cambio
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // Si el estado ha cambiado
    if (reading != buttonState) {
      buttonState = reading;

      // Si el botón está presionado (LOW)
      if (buttonState == LOW) {
        String mensaje = "Alerta presencia de FUEGO EMERGENCIA";

        Callmebot.whatsappMessage(phoneNumber, apiKey, mensaje);
        Serial.println("Mensaje enviado: " + mensaje);
        Serial.println(Callmebot.debug());

        delay(1000);  // Esperar para evitar múltiples envíos
      }
    }
  }

  lastButtonState = reading;  // Actualizar el último estado del botón
}