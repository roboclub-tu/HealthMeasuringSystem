#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h"
#include "WiFi.h"
#include "ESPAsyncWebServer.h"



MAX30105 particleSensor;

const byte RATE_SIZE = 4; 
byte rates[RATE_SIZE]; 
byte rateSpot = 0;
long lastBeat = 0; 
long irValue = 0;
long delta = 0;
 
float beatsPerMinute;
int beatAvg;

void setup() {
  Serial.begin(115200);
  Serial.println("Initializing...");
 
  // Initialize sensor
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) 
  {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1);
  }
  Serial.println("Place your index finger on the sensor with steady pressure.");
 
  particleSensor.setup(); 
  particleSensor.setPulseAmplitudeRed(0x0A); 
  particleSensor.setPulseAmplitudeGreen(0); 

}

void loop() {
  // put your main code here, to run repeatedly:
//uint32_t getIR(void);
//Serial.print("test");
irValue = particleSensor.getIR();
if (irValue < 50000)
  {
    beatAvg = 0;
    beatsPerMinute = 0;
    Serial.print(" No finger?");
  }
else if (checkForBeat(irValue) == true)
  {
    delta = millis() - lastBeat;
    lastBeat = millis();
 
    beatsPerMinute = 60 / (delta / 1000.0);
 
    if (beatsPerMinute < 255 && beatsPerMinute > 20)
    {
      rates[rateSpot++] = (byte)beatsPerMinute; 
      rateSpot %= RATE_SIZE; 
 
 
      beatAvg = 0;
      for (byte x = 0 ; x < RATE_SIZE ; x++)
      {
        beatAvg += rates[x];
      } 
      beatAvg /= RATE_SIZE;
    }
  }

Serial.print("IR=");
  Serial.print(irValue);
  Serial.print(", BPM=");
  Serial.print(beatsPerMinute);
  Serial.print(", Avg BPM=");
  Serial.print(beatAvg);

  
  Serial.println();
}
