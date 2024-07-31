#include "../include/requirements.hpp"
#include "../lib/IRtools.hpp"
#include "../lib/WiFitools.hpp"

decode_results results;

void setup()
{
  EEPROM.begin(X * COM_COUNT);
  Serial.begin(115200);
  irsend.begin();
  irrecv.enableIRIn();
  setupWiFi();
  /*
  sendCode(0);
  delay(5000);
  sendCode(1);
  */
}
void loop()
{
  if (irrecv.decode(&results))
    saveCode(&results, 1);
  loopWiFi();
}