// Extraído de ladyada en
// https://randomnerdtutorials.com/esp32-dht11-dht22-temperature-humidity-sensor-arduino-ide/
// por Juanjo Beunza.

#include "DHT.h"
#define DHTPIN 13
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
Serial.begin(115200);
Serial.println(F("DHTxx test!"));
dht.begin();
}

void loop() {
delay(2000); //Es un sensor lento, por lo que hay que darle tiempo.
float h = dht.readHumidity();
float t = dht.readTemperature();

if (isnan(h) || isnan(t)) {
Serial.println(F("Failed to read from DHT sensor!"));
return;
}

Serial.print(F("Humedad: "));
Serial.print(h);
Serial.print(F("% Temperatura: "));
Serial.print(t);
Serial.println(F("°C "));
}