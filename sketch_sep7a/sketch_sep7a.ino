#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>

 
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET -1
Adafruit_SH1106 display(OLED_RESET);
int i = 0;

 
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  display.begin(SH1106_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();

  Serial.begin(115200);
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
 
}
 
void loop() {
display.clearDisplay();
display.setTextSize(2);
display.setTextColor(WHITE);
display.setCursor(1,8);
if (i == 0)
{
  display.println("   Ajke ");
  display.println("  Shakib ");
  display.println("   -er  ");
  display.display();
  delay(1000);
  i=1;
}
else if (i == 1)
{
  display.println("   Holud ");
  display.println("  Shondha ");
  display.display();
  delay(1000);
  i=0;
}
  
}

void loop() {
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
  Serial.print("X = "); 
  Serial.print(xa);
  Serial.print(" g"); 
  Serial.println(); 


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
  Serial.print("Y = "); 
  Serial.print(ya);
  Serial.print(" g"); 
  Serial.println(); 

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
  Serial.print("Z = "); 
  Serial.print(za);
  Serial.print(" g"); 
  Serial.println(); 
  Serial.println(); 
  delay(500);
}