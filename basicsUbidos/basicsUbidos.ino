#include "Ubidots.h"

#define safeLedPin D3
#define dangerLedPin D4
#define buzerPin D5

#define sensorPin A0
 
const char* UBIDOTS_TOKEN = "BBUS-ufWTVfVq4OiBCXk04Qdib1O8OBY8RW";  // Put here your Ubidots TOKEN
const char* WIFI_SSID = "_AZMAIN_OFFICIAL_";      // Put here your Wi-Fi SSID
const char* WIFI_PASS = "ABBU AMMU SUMAIYA RUBAYED";      // Put here your Wi-Fi password 

Ubidots ubidots(UBIDOTS_TOKEN, UBI_HTTP);

int GasValue = 0;
int smock = 0;

void setup() {                       

  Serial.begin(115200);
  ubidots.wifiConnect(WIFI_SSID, WIFI_PASS);
  ubidots.setDebug(true);
  pinMode(safeLedPin, OUTPUT);
  pinMode(dangerLedPin, OUTPUT);
  pinMode(buzerPin, OUTPUT);
  pinMode(sensorPin, INPUT);
  digitalWrite(safeLedPin, HIGH);
  digitalWrite(dangerLedPin, LOW);
  digitalWrite(buzerPin, LOW);
}

void loop() {
  GasValue = analogRead(sensorPin);
  Serial.println(GasValue);


  if (GasValue > 87){
    digitalWrite(safeLedPin, LOW);
    digitalWrite(dangerLedPin, HIGH);
    digitalWrite(buzerPin, HIGH);
    smock = 1;
    ubidots.add("gas-value", GasValue);
    ubidots.add("is-leaked", smock);
  
    bool bufferSent = false;
    bufferSent = ubidots.send(); 

    if (bufferSent) {
      Serial.println("Values sent by the device");
    }
     smock = 0;
     ubidots.add("is-leaked", smock);
  
    bufferSent = false;
    bufferSent = ubidots.send(); 

    if (bufferSent) {
      Serial.println("Values sent by the device");
    }
  }else{
    digitalWrite(safeLedPin, HIGH);
    digitalWrite(dangerLedPin, LOW);
    digitalWrite(buzerPin, LOW);
    smock = 0;
  }
  delay(10); 

}