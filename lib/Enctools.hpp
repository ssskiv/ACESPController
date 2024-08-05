#include "requirements.hpp"

#include <EncButton.h>

#define DT 33
#define CLK 32
#define BT 25

EncButton encoder(CLK, DT, BT);
int counter = 0;
int limit = 0;

void isr()
{
    encoder.tickISR();
}

void setupEnc(int _limit = COM_COUNT * 2 + 1)
{
    encoder.setEncType(EB_STEP2);
    limit = _limit - 1;
    attachInterrupt(CLK, isr, CHANGE);
    attachInterrupt(DT, isr, CHANGE);
    encoder.setEncISR(true);
}

void loopEnc()
{

    if (encoder.tick())
    {
        if (encoder.turn())
        {
            counter += encoder.dir();
            if (counter > limit)
                counter = 0;
            else if (counter < 0)
                counter = limit;
            // Serial.println(counter);
        }
        if (encoder.click())
        {
            executeItem( counter);
            Serial.println("Clicked");
        }
        Serial.println(encoder.action());
    }
}
