#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>
#include "MAX30105.h"

MAX30105 particleSensor;

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET -1
Adafruit_SH1106 display(OLED_RESET);

void setup() {
  Serial.begin(115200);
  if (particleSensor.begin() == false)
  {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1);
  }

  particleSensor.setup();

  Wire.begin();
  Wire.beginTransmission(0x53);
  Wire.write(0x2C); 
  Wire.write(0x08); 
  Wire.endTransmission(); 

  Wire.beginTransmission(0x53);
  Wire.write(0x31); 
  Wire.write(0x08); 
  Wire.endTransmission();

  Wire.beginTransmission(0x53);
  Wire.write(0x2D); 
  Wire.write(0x08); 
  Wire.endTransmission();

  display.begin(SH1106_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(WHITE);
  for (int i = SCREEN_HEIGHT+45 ; i >45;i-- )
  {
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(30,i);
    display.print("CARE+");
    display.setCursor(35,i-20);
    display.print("LIFE");
    display.setCursor(20,i-40);
    display.print("OCTAGON");
    display.display();
    delay(10);
    
  }
  delay(2000);
  display.clearDisplay();
  display.display();
}

void loop() {

  display.clearDisplay();

  Wire.beginTransmission(0x53);
  Wire.write(0x32); 
  Wire.endTransmission();
  Wire.requestFrom(0x53, 1);
  byte x0 = Wire.read();

  Wire.beginTransmission(0x53);
  Wire.write(0x33); 
  Wire.endTransmission();
  Wire.requestFrom(0x53, 1);
  byte x1 = Wire.read();
  x1 = x1 & 0x03;

  uint16_t x = (x1 << 8) + x0;
  int16_t xf = x;
  if(xf > 511)
  {
    xf = xf - 1024;
  }
  float xa = xf * 0.004;

  display.setTextSize(1);
  display.setCursor(0,55);
  display.print("X:");
  display.print(xa);
  
  Wire.beginTransmission(0x53);
  Wire.write(0x34); 
  Wire.endTransmission();
  Wire.requestFrom(0x53, 1);
  byte y0 = Wire.read();

  Wire.beginTransmission(0x53);
  Wire.write(0x35); 
  Wire.endTransmission();
  Wire.requestFrom(0x53, 1);
  byte y1 = Wire.read();
  y1 = y1 & 0x03;

  uint16_t y = (y1 << 8) + y0;
  int16_t yf = y;
  if(yf > 511)
  {
    yf = yf - 1024;
  }
  float ya = yf * 0.004;

  display.setTextSize(1);
  display.setCursor(40,55);
  display.print("Y:");
  display.print(ya);

  Wire.beginTransmission(0x53);
  Wire.write(0x36); 
  Wire.endTransmission();
  Wire.requestFrom(0x53, 1);
  byte z0 = Wire.read();

  Wire.beginTransmission(0x53);
  Wire.write(0x37); 
  Wire.endTransmission();
  Wire.requestFrom(0x53, 1);
  byte z1 = Wire.read();
  z1 = z1 & 0x03;

  uint16_t z = (z1 << 8) + z0;
  int16_t zf = z;
  if(zf > 511)
  {
    zf = zf - 1024;
  }
  float za = zf * 0.004;

  display.setTextSize(1);
  display.setCursor(85,55);
  display.print("Z:");
  display.print(za);

  float at = sqrt(xa*xa + ya*ya + za*za);

  display.setTextSize(1);
  display.setCursor(20,20);
  display.print("total Acc:");
  display.print(at);

  display.setTextSize(1);
  display.setCursor(0,30);
  display.print("R[");
  display.print(particleSensor.getRed());
  display.print("] ");
  display.setCursor(50,30);
  display.print("IR[");
  display.print(particleSensor.getIR());
  display.print("]");
  display.setCursor(0,40);
  display.print("G[");
  display.print(particleSensor.getGreen());
  display.print("]");

  if (at<0.15){
    display.setTextSize(1);
    display.setCursor(9,0);
    display.print(" fall detected !");
    display.display();
    delay(8000);
  }else{
    display.setTextSize(1);
    display.setCursor(5,0);
    display.print(" no fall detected !");
  }

  display.display();
  delay(10);
}