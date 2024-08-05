#include "requirements.hpp"

#define TEMP1_PIN 18

OneWire oneWire(TEMP1_PIN);

float readTemperature() {
  byte i;
  byte present = 0;
  byte type_s;
  byte data[12];
  byte addr[8];
 
  // Поиск подключенных датчиков DS18B20 на шине OneWire
  if (!oneWire.search(addr)) {
    //Serial.println("Датчики DS18B20 не найдены.");
    // Сбрасываем поиск и возвращаем "NaN" (не число) в случае ошибки
    oneWire.reset_search();
    return NAN;
  }
 
  // Проверка целостности адреса датчика с помощью CRC
  if (OneWire::crc8(addr, 7) != addr[7]) {
    Serial.println("CRC не совпадает!");
    return NAN;
  }
 
  // Определение типа датчика на основе первого байта адреса
  switch (addr[0]) {
    case 0x10:
      type_s = 1; // DS18S20 или DS1822
      break;
    case 0x28:
    case 0x22:
      type_s = 0; // DS18B20
      break;
    default:
      Serial.println("Неизвестный тип датчика.");
      return NAN;
  }
 
  // Сбрасываем шину и выбираем адрес конкретного датчика
  oneWire.reset();
  oneWire.select(addr);
  
  // Запускаем измерение температуры на датчике
  oneWire.write(0x44); // 0x44 - команда начать измерение
 
  // Ожидание завершения измерения (время зависит от разрешения)
  delay(1000);
 
  // Сбрасываем шину и выбираем адрес датчика для чтения данных
  present = oneWire.reset();
  oneWire.select(addr);
  oneWire.write(0xBE); // 0xBE - команда чтения данных
 
  // Считываем 9 байт данных температуры и CRC
  for (i = 0; i < 9; i++) {
    data[i] = oneWire.read();
  }
 
  // Преобразование считанных данных в температуру
  int16_t raw = (data[1] << 8) | data[0];
  float celsius = 0.0;
 
  // Применяем разрешение и коррекцию для разных типов датчиков
  if (type_s == 1) {
    raw = raw << 3; // Увеличиваем разрешение до 12 бит
    if (data[7] == 0x10) {
      // Температура с высоким разрешением (DS18S20)
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } else {
    byte cfg = (data[4] & 0x60);
    if (cfg == 0x00) raw = raw & ~7; // Разрешение 9 бит, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // Разрешение 10 бит, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // Разрешение 11 бит, 375 ms
  }
  celsius = (float)raw / 16.0; // Преобразуем вещественное значение температуры и возвращаем
 
  return celsius;
}

void setupSensors()
{
    
}


