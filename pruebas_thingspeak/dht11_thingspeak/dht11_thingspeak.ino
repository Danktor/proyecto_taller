#include "ThingSpeak.h"
unsigned long myChannel = 2528368;
const char* writeAPIKey = "N2KPI65GM2XFGSPF";
#include <ESP8266WiFi.h>
#include "DHT.h"

#define DHTPIN 13
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

char ssid[] = "Diego Forlong";
char pass[] = "milumorita2003";
WiFiClient client;

void setup() {
  Serial.begin(115200);
  Serial.println(F("DHTxx test!"));
  delay(100);
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);
  dht.begin();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to Wifi...");
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, pass);
      delay(5000);
    }
  }
  delay(2000);
  long rssi = WiFi.RSSI();
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
  ThingSpeak.setField(1, rssi);
  ThingSpeak.setField(2, t);
  ThingSpeak.setField(3, h);
  int httpCode = ThingSpeak.writeFields(myChannel,writeAPIKey);
  if (httpCode == 200) {
    Serial.println("Channel write successful.");
  } else {
    Serial.println("HTTP error code " + String(httpCode));
  }
  delay(20000);
}
