#define USE_ARDUINO_INTERRUPTS true
#include <PulseSensorPlayground.h>  

//  Variables
const int PulseWire = 0;
const int LED = 13;          
int Threshold = 550;   
 
                               
PulseSensorPlayground pulseSensor;  


void setup() {   

  Serial.begin(9600);         

  pulseSensor.analogInput(PulseWire);   
  pulseSensor.blinkOnPulse(LED);     
  pulseSensor.setThreshold(Threshold);   

   if (pulseSensor.begin()) {
    Serial.println("We created a pulseSensor Object !");  
  }
}



void loop() {

 

  if (pulseSensor.sawStartOfBeat()) {          
    int myBPM = pulseSensor.getBeatsPerMinute();                          
    Serial.println(myBPM);                       
  }

  delay(20);                    

}

  
