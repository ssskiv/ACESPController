#include "requirements.hpp"

LiquidCrystal_I2C lcd(0x27, 16, 2);

const String menuItems[] = {"info", "save 0", "save 1", "save 2", "save 3", "send 0", "send 1", "send 2", "send 3"};
uint8_t len = sizeof(menuItems) / sizeof(menuItems[0]);
short menuIter = 0;
short mainMenuIter = 0;

decode_results results;

void setupLCD()
{
    lcd.init();
    lcd.backlight();
    lcd.clear();
}

void printMenu(int selectedPos = mainMenuIter, const String currentMenuItems[] = menuItems)
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(">" + menuItems[selectedPos]);
    lcd.setCursor(1, 1);
    if (selectedPos == len - 1)
        lcd.print(menuItems[0]);
    else
        lcd.print(menuItems[selectedPos + 1]);
    delay(20);
}

void nextItem()
{
    mainMenuIter++;
    if (mainMenuIter > len - 1)
        mainMenuIter = 0;
}

void prevItem()
{
    mainMenuIter--;
    if (mainMenuIter < 0)
        mainMenuIter = len - 1;
}

void lcdSaveCode(int idx)
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Waiting code...");
    while (true)
    {
        if (irrecv.decode(&results))
            if (!results.repeat)
            {
                saveCode(&results, idx);
                break;
            }
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Saved to " + String(idx));
    delay(900);
}

void lcdSendCode(int idx)
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sending...");
    sendCode(idx);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Ready");
    delay(500);
}

void executeItem(int selectedPos = mainMenuIter)
{
    Serial.println("Executing " + String(selectedPos));
    if (selectedPos >= 1 && selectedPos <= 4)
    {
        lcdSaveCode(selectedPos - 1);
    }
    if (selectedPos >= 5)
    {
        lcdSendCode(selectedPos - 5);
    }
}
