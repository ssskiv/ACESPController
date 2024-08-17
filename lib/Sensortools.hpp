#include "requirements.hpp"

OneWire temp0w(TEMP0_PIN);
DallasTemperature temp0(&temp0w);

OneWire temp1w(TEMP1_PIN);
DallasTemperature temp1(&temp1w);


DHT temp2(TEMP2_PIN, DHT11);

 

float readTemperature(int idx)
{
  static uint32_t tmr;
  if (millis() - tmr >= 800)
  {
    //Serial.println("Getting temperature...");
    tmr = millis();
    float temps[] = {temp0.getTempCByIndex(0), temp1.getTempCByIndex(0), temp2.readTemperature()};
    temp0.requestTemperatures();
    temp1.requestTemperatures();

    return temps[idx];
  }
  else
    return 0;
}

float readHumidity(){
  return temp2.readHumidity();
}

void setupSensors()
{
  temp0.begin();
  temp1.begin();
  temp2.begin();
  pinMode(WAT0_PIN, INPUT);
  pinMode(WAT1_PIN, INPUT);
  pinMode(TEMP2_PIN, INPUT);
}

bool readWater(int idx)
{
  if (idx == 0)
  {
    if (analogRead(WAT0_PIN) <= 1000)
      return true;
    return false;
  }
  else
  {
    if (analogRead(WAT1_PIN) <= 1000)
      return true;
    return false;
  }
}