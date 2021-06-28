#ifndef DisplayTools_H
#define DisplayTools_H

#include "Arduino.h"
#include "LiquidCrystal_I2C.h"
#include "Wire.h"

class DisplayTools
{
  private:
  static int buttonPress;
  static void interruptNoButton();
  static void interruptYesButton();

  public:
  static LiquidCrystal_I2C *lcd;
  static void init(LiquidCrystal_I2C *lcd);

  static void print(String text);
  
  static bool prompt(char prompt[16], char yes[7], char no[7], int noPin, int yesPin);
};

#endif