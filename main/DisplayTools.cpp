#include "DisplayTools.h"
#include "Arduino.h"
#include "LiquidCrystal_I2C.h"
#include "Wire.h"

DisplayTools::DisplayTools()
{
  lcd.init();
  lcd.backlight();
}

void DisplayTools::print(String text) {
  if (text.length() > 16) {
    lcd.autoscroll();
  } else {
    lcd.noAutoscroll();
  }
  lcd.print(text);
}

bool DisplayTools::prompt(String prompt, String yes, String no, int noPin, int yesPin) {
  lcd.noAutoscroll();
  lcd.setCursor(8, 1);
  lcd.print(no);
  lcd.setCursor(0, 1);
  lcd.print(yes);
  lcd.setCursor(0, 0);
  lcd.print(prompt);
  while(digitalRead(noPin) == 1 || digitalRead(yesPin) == 1) {}
  if(digitalRead(noPin) == 1)
  {
    return false;
  }
  else
  {
    return true;
  }
}