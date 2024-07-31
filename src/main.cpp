#include "requirements.hpp"
#include "IRtools.hpp"

decode_results results;

void setup()
{
  EEPROM.begin(X * COM_COUNT);
  Serial.begin(115200);
  irsend.begin();
  irrecv.enableIRIn();

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
}