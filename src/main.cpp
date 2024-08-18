#include "../include/requirements.hpp"
#include "../lib/IRtools.hpp"
#include "../lib/LCDtools.hpp"
#include "../lib/Enctools.hpp"
#include "../lib/Sensortools.hpp"
#include "../lib/Ethtools.hpp"
// decode_results results;
//  int state = 0;

void setup()
{
  EEPROM.begin(X * COM_COUNT+200);
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
int __time;
float tep0 = 0;
float tep1 = 0;
float tep2 = 0;
bool water0 = false;
bool water1 = true;
float humidity = 0;

void baseLoop()
{
  if (loopEnc())
    printMenu(counter);

  if (millis() - __time >= 10000)
  {
    __time = millis();
    tep0 = readTemperature(0);
    tep1 = readTemperature(1);
    tep2 = readTemperature(2);
    //!!water
    humidity = readHumidity();
    water0 = readWater(0);
    water1 = readWater(1);
  }
  loopETH(tep0, tep1, tep2, water0, water1, humidity);
}

void loop()
{
  baseLoop();
  // your logic here
  /* You can use next commands:
  sendCode(int idx); idx - int from 0 to 3. 0 - on AC1; 1 - off AC1; 2 - on AC2; 3 - off AC2.
  get sensors as tep0, water0, humidity etc.
  ! DO NOT USE DELAY IN ANY FORMS!!!!
  ! IT WILL BREAK ENCODER AND ETHERNET IMMEDIATELY!!!

  */
}