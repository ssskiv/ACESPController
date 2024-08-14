#include "../include/requirements.hpp"
#include "../lib/IRtools.hpp"
#include "../lib/LCDtools.hpp"
#include "../lib/Enctools.hpp"
#include "../lib/Sensortools.hpp"

// decode_results results;
int state = 0;

void setup()
{
  EEPROM.begin(X * COM_COUNT);
  Serial.begin(115200);
  setupSensors();
  setupIR();
  setupLCD();
  setupEnc(9);

  // sendCode(3);
  // delay(5000);
  // Serial.println(resultToHumanReadableBasic(results))
  // sendCode(1);
}

void loop()
{
  loopEnc();
  printMenu(counter);
  Serial.println(readTemperature(0));
  readWater(0);
}