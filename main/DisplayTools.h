#ifndef DisplayTools_H
#define DisplayTools_H

#include "Arduino.h"
#include "LiquidCrystal_I2C.h"
#include "Wire.h"

class DisplayTools
{
  private:
  LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);

  public:
  DisplayTools();
  void init();

  void print(String text);
  
  bool prompt(String prompt, String yes, String no, int noPin, int yesPin);
};

#endif