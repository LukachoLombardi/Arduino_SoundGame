#include "DisplayTools.h"
#include "Arduino.h"
#include "LiquidCrystal_I2C.h"
#include "Wire.h"
  void DisplayTools::interruptNoButton()
  {
    buttonPress = 0;
  }
  void DisplayTools::interruptYesButton()
  {
    buttonPress = 1;
  }
  void DisplayTools::init(LiquidCrystal_I2C *lcd)
  {
    DisplayTools::lcd = lcd;
  }

  void DisplayTools::print(String text)
  {
    if(text.length() > 16)
    {
      lcd->autoscroll();
    }
    else
    {
      lcd->noAutoscroll();
    }
    lcd->print(text);
  }
  
  bool DisplayTools::prompt(char prompt[16], char yes[7], char no[7], int noPin, int yesPin)
  {
    lcd->noAutoscroll();
    lcd->setCursor(8,1);
    lcd->print(no);
    lcd->setCursor(0,1);
    lcd->print(yes);
    lcd->setCursor(0,0);
    lcd->print(prompt);
    buttonPress = 2;
    attachInterrupt(digitalPinToInterrupt(noPin), interruptNoButton, LOW);
    attachInterrupt(digitalPinToInterrupt(yesPin), interruptYesButton, LOW);
    while(buttonPress == 2){}
    switch(buttonPress)
    {
      case 0: return false; break;
      case 1: return true; break;
    }
  }