#include "../include/requirements.hpp"
#include "../lib/IRtools.hpp"
#include "../lib/LCDtools.hpp"
#include "../lib/Enctools.hpp"
#include "../lib/Sensortools.hpp"
#include "../lib/Ethtools.hpp"
//decode_results results;
// int state = 0;

void setup()
{
  EEPROM.begin(X * COM_COUNT);
   Serial.begin(115200);
  setupSensors();
  setupIR();

   setupEnc(9);
  setupETH();
  // delay(200);
  setupLCD();
   printMenu(counter);
  // sendCode(3);
  // delay(5000);
  // //Serial.println(resultToHumanReadableBasic(results))
  // sendCode(1);
}

void loop()
{
  if (loopEnc())
    printMenu(counter);
   loopETH();
  //  readWater(0);
}