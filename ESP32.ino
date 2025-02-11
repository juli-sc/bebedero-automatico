#include <WiFi.h>
#include <Callmebot_ESP32.h>
#include <HTTPClient.h>

int relePin = 23;         // Pin conectado a IN del relé
int sensorPin = 32;      // Pin conectado a la salida del sensor de agua
int umbral = 300;        // Valor umbral para detectar la presencia de agua (ajusta este valor según tu sensor)

const int echo = 18;
const int trigger = 5;
long tiempo;
float distancia;
int porc = 0;

// Configuración WiFi
const char* ssid = "Juli";
const char* password = "jorgeroberto";

// Configuración de CallMeBot para enviar WhatsApp
String phoneNumber = "54..........";  // Número de teléfono con código de país
String apiKey = "..........";             // API Key de CallMeBot
String mensaje = "bebedero sin agua";  // Mensaje de alerta

void setup() {
  pinMode(relePin, OUTPUT);
  digitalWrite(relePin, LOW); // Inicializa el relé en estado apagado
  Serial.begin(9600);         // Inicializa la comunicación serial para depuración

  pinMode(echo, INPUT);
  pinMode(trigger, OUTPUT);

  // Conexión WiFi
  WiFi.begin(ssid, password);
  Serial.print("Conectando a WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConectado a WiFi");
}

void loop() {
  medir();          // Realiza la medición de distancia
  porcentaje();     // Calcula el porcentaje de distancia
  int lecturaSensor = analogRead(sensorPin); // Lee el valor del sensor de agua

  Serial.print("Nivel de agua: ");
  Serial.println(lecturaSensor);
  Serial.print("Distancia: ");
  Serial.print(distancia);
  Serial.print(" cm, Porcentaje: ");
  Serial.print(porc);
  Serial.println(" %");

  // Control del relé según el umbral y el porcentaje del sensor ultrasónico
  if (lecturaSensor < umbral && porc > 25) {    // Solo activa el relé si el porcentaje es mayor al 60%
    digitalWrite(relePin, HIGH);                // Activa el relé (enciende la bomba)
    Serial.println("No hay agua suficiente, encendiendo bomba.");
  } else {
    digitalWrite(relePin, LOW);                 // Apaga el relé (apaga la bomba)
    Serial.println("Suficiente agua o nivel bajo detectado, apagando bomba.");
  }

  // Enviar mensaje de alerta si el porcentaje es menor o igual al 40%
  if (porc <= 40) {
    alarma();
  }

  delay(1000); // Espera 1 segundo antes de verificar nuevamente
}

// Función de alarma que envía un mensaje de WhatsApp usando CallMeBot
void alarma() {
  if (WiFi.status() == WL_CONNECTED) { // Verifica que esté conectado a WiFi
    Callmebot.whatsappMessage(phoneNumber, apiKey, mensaje);
    Serial.println("Mensaje de alerta enviado: " + mensaje);
    Serial.println(Callmebot.debug());
  } else {
    Serial.println("Error en la conexión WiFi");
  }
}

void porcentaje() {
  porc = map(distancia, 5, 23, 100, 0); // Convertir de cm a %
}

void medir() {
  digitalWrite(trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);

  tiempo = pulseIn(echo, HIGH); // us=microsegundos
  distancia = float(tiempo * 0.0343) / 2;
  delay(10);
}
