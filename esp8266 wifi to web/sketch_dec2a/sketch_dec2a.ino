#ifdef ESP32
  #include <WiFi.h>
  #include <HTTPClient.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESP8266HTTPClient.h>
  #include <WiFiClient.h>
#endif

#include <Wire.h>

const char* ssid     = "_AZMAIN_OFFICIAL_";
const char* password = "ABBU AMMU SUMAIYA RUBAYED";


const char* serverName = "http://mystrix.site/esp/post-esp-data.php";


String apiKeyValue = "tPmAT5Ab3j7F9";
String sensorName = "IR";
String sensorLocation = "My Room";

const int irSensorPin = D5;

int demo=1;
int count=1;


void setup() {
  Serial.begin(115200);
  
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  pinMode(irSensorPin ,INPUT);

  
}
void loop() {

  int irValue = digitalRead(irSensorPin);
  //Check WiFi connection status
  if(WiFi.status()== WL_CONNECTED){
    HTTPClient http;
    WiFiClient client; 

    http.begin(client, serverName);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    
    // Prepare your HTTP POST request data
String httpRequestData = "api_key=" + apiKeyValue + "&sensor=" + sensorName +
                             "&location=" + sensorLocation + "&value1=" + String(irValue) +
                             "&value2=" + String(demo) + "&value3=" + String(count);
    
    Serial.print("httpRequestData: ");
    Serial.println(httpRequestData);
    

    int httpResponseCode = http.POST(httpRequestData);
    count++;
     
  if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }
  //Send an HTTP POST request every 15 seconds
  delay(5000);  
}