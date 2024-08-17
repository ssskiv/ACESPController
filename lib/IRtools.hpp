#include "../include/requirements.hpp"

IRrecv irrecv(RCV_PIN, BUF_LEN, 50, true);
IRsend irsend0(SND0_PIN);
IRsend irsend1(SND1_PIN);

void setupIR()
{
    irsend0.begin();
    irsend1.begin();
    irrecv.enableIRIn();
    pinMode(RCV_PIN, INPUT);
}

struct irData
{
    uint16_t size;
    uint16_t rawCodes[BUF_LEN] = {0};
};

void saveCode(decode_results *res, int idx)
{
    irData data;
    uint16_t *raw_array = resultToRawArray(res);
    data.size = getCorrectedRawLength(res);
    for (int i = 0; i < data.size; i++)
        data.rawCodes[i] = raw_array[i];
    EEPROM.put(idx * X, data);
    EEPROM.commit();
    delete[] raw_array;
}

void sendCode(int idx, int sndIdx)
{
    irrecv.pause();
    irData data;
    EEPROM.get(idx * X, data);
    
    if (sndIdx == 0)
        irsend0.sendRaw(data.rawCodes, data.size, 38);
    else
        irsend1.sendRaw(data.rawCodes, data.size, 38);
    irrecv.resume();
    Serial.println("sent code "+String(idx)+" from "+sndIdx);
}

void sendCode(int idx)
{
    if (idx == 0 or idx == 1)
        sendCode(idx, 0);
    else
        sendCode(idx, 1);
}
