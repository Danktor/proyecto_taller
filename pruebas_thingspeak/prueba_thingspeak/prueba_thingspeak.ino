#include "ThingSpeak.h"

unsigned long myChannel = 2528368 ;
const char* writeAPIKey = "N2KPI65GM2XFGSPF" ;

#include <ESP8266WiFi.h>

char ssid[] = "Diego Forlong" ;
char pass[] = "milumorita2003" ;

WiFiClient client;

void setup() {
    Serial.begin(115200);
    delay(100);
    WiFi.mode(WIFI_STA );
    ThingSpeak .begin(client);
}
void loop() {
    if (WiFi.status() != WL_CONNECTED ) {
        Serial.print("Attempting to connect to Wifi..." );
        while (WiFi.status() != WL_CONNECTED ) {
        WiFi.begin(ssid, pass );
        delay(5000);
    }
    }
    long rssi = WiFi.RSSI();
    int httpCode = ThingSpeak .writeField (myChannel, 1, rssi,
    writeAPIKey );
    if (httpCode == 200) {
        Serial.println ("Channel write successful." );
    } else {
        Serial.println ("HTTP error code " + String(httpCode ));
    }
    delay(20000);
}