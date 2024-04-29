#include "DHT.h"

const int trigPin = 12;
const int echoPin = 14;
const int trigPin_2 = 5;
const int echoPin_2 = 4;

// Define sound velocity in cm/uS
#define SOUND_VELOCITY 0.034
#define CM_TO_INCH 0.393701

#define DHTPIN 13
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

long duration;
float distanceCm;
float distanceInch;

void setup() {
  Serial.begin(115200); // Inicia la comunicación serial

  pinMode(trigPin, OUTPUT); // Establece el trigPin como salida
  pinMode(echoPin, INPUT);  // Establece el echoPin como entrada
  pinMode(trigPin_2, OUTPUT); // Establece el trigPin_2 como salida
  pinMode(echoPin_2, INPUT);  // Establece el echoPin_2 como entrada

  Serial.println(F("DHTxx test!")); // Inicia el sensor DHT
  dht.begin();
}

void loop() {
  // Sensor ultrasónico 1
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distanceCm = duration * SOUND_VELOCITY / 2;
  Serial.print("Distance 1 (cm): ");
  Serial.println(distanceCm);

  // Sensor ultrasónico 2
  digitalWrite(trigPin_2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin_2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin_2, LOW);
  duration = pulseIn(echoPin_2, HIGH);
  distanceCm = duration * SOUND_VELOCITY / 2;
  Serial.print("Distance 2 (cm): ");
  Serial.println(distanceCm);

  // Sensor de temperatura y humedad DHT11
  delay(2000); // Espera 2 segundos para obtener una nueva lectura
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println(F("No puedo leer del DHT11!"));
    return;
  }

  Serial.print(F("Humedad: "));
  Serial.print(h);
  Serial.print(F("% Temperatura: "));
  Serial.print(t);
  Serial.println(F("°C "));

  Serial.println("Devuelta\n\n");
  
  delay(3000); // Repite en 3 segundos
}
