#include <LiquidCrystal_I2C.h>
#include "DHT.h"
#include "ThingSpeak.h"
unsigned long myChannel = 2548553;
const char* writeAPIKey = "IC4NLED30JTWYGSR";
#include <ESP8266WiFi.h>
// 1 cable macho/hembra 1 h/h
const int trigPin = 12;
const int echoPin = 14;
const int trigPin_2 = 15;
const int echoPin_2 = 16;

const int medida = 125; //distancia a las personas 
const int capacidad = 8; // capacidad del salon
// Define sound velocity in cm/uS
#define SOUND_VELOCITY 0.034
#define CM_TO_INCH 0.393701

// set the LCD number of columns and rows
int lcdColumns = 16;
int lcdRows = 2;

// set LCD address, number of columns and rows
// if you don't know your display address, run an I2C scanner sketch
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows); 

char ssid[] = "wifing";
char pass[] = "wifing-pub";
WiFiClient client;

#define DHTPIN 13
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
int iteraciones_actuador = 0;
int actuador = 0;
int entrada = 0;
int salida = 0;
int repeticiones = 0;
int cant_personas = 0;
long duration;
float distanceCm_1anterior = 0;
float distanceCm_2anterior = 0;
float distanceCm_1;
float distanceCm_2;
int contador = 0;
void setup() {
  Serial.begin(115200); // Inicia la comunicación serial
  // initialize LCD
  lcd.init();
  // turn on LCD backlight                      
  lcd.backlight();
  pinMode(trigPin, OUTPUT); // Establece el trigPin como salida
  pinMode(echoPin, INPUT);  // Establece el echoPin como entrada
  pinMode(trigPin_2, OUTPUT); // Establece el trigPin_2 como salida
  pinMode(echoPin_2, INPUT);  // Establece el echoPin_2 como entrada

  Serial.println(F("DHTxx test!"));
  delay(100);
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);
  dht.begin();

  Serial.println(F("DHTxx test!")); // Inicia el sensor DHT
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
  // Sensor ultrasónico 1
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distanceCm_1 = duration * SOUND_VELOCITY / 2;
  digitalWrite(trigPin_2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin_2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin_2, LOW);
  duration = pulseIn(echoPin_2, HIGH);
  distanceCm_2 = duration * SOUND_VELOCITY / 2;
  
  Serial.print("Distance 1 (cm): ");
  Serial.println(distanceCm_1);
  Serial.print("Distance 2 (cm): ");
  Serial.println(distanceCm_2);
  //if(distanceCm_1anterior==1&&distanceCm_2anterior==1){
    //distanceCm_1anterior = 0;
    //distanceCm_2anterior = 0;
  //}
  //else{
  if(distanceCm_1<medida){
    if(distanceCm_2anterior==1){
      cant_personas--;
      distanceCm_2anterior = 0;
      distanceCm_1anterior = 0;
    }
    else{
    distanceCm_1anterior = 1;
    distanceCm_2anterior = 0;
    }
  }
  if(distanceCm_2<medida){
    if(distanceCm_1anterior==1){
      cant_personas++;
      distanceCm_1anterior = 0;
      distanceCm_2anterior = 0;
    }
    else{
    distanceCm_2anterior = 1;
    distanceCm_1anterior = 0;
    }
  }
  //}
  float t = dht.readTemperature();
  if (isnan(t)) {
    Serial.println(F("No puedo leer del DHT11!"));
    return;
  }
  if(contador>50){
  ThingSpeak.setField(1, t);
  ThingSpeak.setField(2, cant_personas);
  ThingSpeak.setField(3, actuador);
  int httpCode = ThingSpeak.writeFields(myChannel,writeAPIKey);
  if (httpCode == 200) {
    Serial.println("Channel write successful.");
  } else {
    Serial.println("HTTP error code " + String(httpCode));
  }
  contador=0;
  }
  Serial.print("Cantidad de personas: ");
  Serial.println(cant_personas);
  Serial.println("Devuelta\n");
  contador++;
  if(t>21){
    iteraciones_actuador++;
  }
  else {
    actuador = 0;
  }
  if(iteraciones_actuador>40){
    actuador = 1;
    iteraciones_actuador = 0;
    lcd.setCursor(0, 0);
    // print message
    lcd.print("Actuador: ON");
    delay(100);
    // clears the display to print new message
    lcd.clear();
    // set cursor to first column, second row
    lcd.setCursor(0,1);
    lcd.print("Actuador: ON");
  }
  else{
    // set cursor to first column, first row
    lcd.setCursor(0, 0);
    // print message
    String stringOne = String(t);
    lcd.print(t);
    delay(100);
    // clears the display to print new message
    lcd.clear();
    // set cursor to first column, second row
    lcd.setCursor(0,1);
    String stringTwo = String(cant_personas);
    lcd.print(cant_personas);
  }
  delay(100);
  lcd.clear(); 
  delay(200);
}
