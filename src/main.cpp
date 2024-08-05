#include "../include/requirements.hpp"
#include "../lib/IRtools.hpp"
#include "../lib/LCDtools.hpp"
#include "../lib/Enctools.hpp"
// #include "../lib/WiFitools.hpp"

//decode_results results;
int state = 0;

void setup()
{
  EEPROM.begin(X * COM_COUNT);
  Serial.begin(115200);
  irsend.begin();
  irrecv.enableIRIn();
  setupLCD();
  setupEnc(9);
  // setupWiFi();

  // sendCode(3);
  // delay(5000);
  // Serial.println(resultToHumanReadableBasic(results))
  // sendCode(1);
}
void loop()
{
  // if (irrecv.decode(&results))
  //   if (!results.repeat)
  //     saveCode(&results, 1);

  loopEnc();
  // if (!encoder.busy())

  printMenu(counter);
  // nextItem();

  // delay(100);//TODO убрать
}