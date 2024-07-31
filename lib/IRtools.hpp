#include "../include/requirements.hpp"

IRrecv irrecv(RCV_PIN, BUF_LEN, 50, true);
IRsend irsend(SND_PIN);

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
}

void sendCode(int idx)
{
    irData data;
    EEPROM.get(idx * X, data);
    irsend.sendRaw(data.rawCodes, data.size, 38);
}
