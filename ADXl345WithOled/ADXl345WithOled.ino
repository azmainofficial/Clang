#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>
#include <Adafruit_Sensor.h> 
#include <Adafruit_ADXL345_U.h>

Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified();

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET -1
Adafruit_SH1106 display(OLED_RESET);

void setup() {
  Serial.begin(115200);
  Wire.begin();
       
  //Wire.beginTransmission(0x53);
  //Wire.write(0x2C); 
  //Wire.write(0x08); 
  //Wire.endTransmission(); 

  //Wire.beginTransmission(0x53);
  //Wire.write(0x31); 
  //Wire.write(0x08); 
  //Wire.endTransmission();

  //Wire.beginTransmission(0x53);
  //Wire.write(0x2D); 
  //Wire.write(0x08); 
  //Wire.endTransmission();
  

  display.begin(SH1106_SWITCHCAPVCC, 0x3C);
  if(!accel.begin())
   {
      Serial.println("No ADXL345 sensor detected.");
      while(1);
   }
  //delay(2000);
  display.clearDisplay();
  display.setTextColor(WHITE);
}

void loop() {
  sensors_event_t event; 
  accel.getEvent(&event);
  delay(100);
  display.clearDisplay();

  /*Wire.beginTransmission(0x53);
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
  float xa = xf * 0.004;*/
  Serial.print("X = "); 
  //Serial.print(xa);
  //Serial.print(" g"); 
  Serial.print(event.acceleration.x); Serial.print("  ");

  display.setTextSize(2);
  display.setCursor(0,5);
  display.print("X: ");
  display.print(event.acceleration.x);
  //display.print(" g");


  /*Wire.beginTransmission(0x53);
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
  float ya = yf * 0.004;*/
  
  Serial.print("Y = "); 
 /* Serial.print(ya);
  Serial.print(" g"); 
  Serial.println(); */
  Serial.print(event.acceleration.y); Serial.print("  ");

  display.setTextSize(2);
  display.setCursor(0,25);
  display.print("Y: ");
  display.print(event.acceleration.y);
  //display.print(" g");

  /*Wire.beginTransmission(0x53);
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
  float za = zf * 0.004;*/

  Serial.print("Z = "); 
  /*Serial.print(za);
  Serial.print(" g"); 
  Serial.println(); */
  Serial.print(event.acceleration.z); Serial.print("  ");
  Serial.println();

  display.setTextSize(2);
  display.setCursor(0,45);
  display.print("Z: ");
  display.print(event.acceleration.z);
  //display.print(" g");

  display.display();
//delay(250);
}