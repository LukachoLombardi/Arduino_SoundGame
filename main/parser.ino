#include "Arduino.h"
class parser
{
  private:
  int * Notes;
  int * Timings;
  int * Directions;
  public:
  enum ReturnableArrays {NOTES,TIMINGS,DIRECTIONS};
  int getSongArray(ReturnableArrays ArrayToReturn)
  {
    switch(ArrayToReturn)
    {
      case NOTES:
      return *Notes;
      break;

      case TIMINGS:
      return *Timings;
      break;

      case DIRECTIONS:
      return *Directions;
      break;
    }
  }
  bool parseChars(char CharsToParse[])
  {
    int CharsLength = (int)sizeof(CharsToParse)/sizeof(CharsToParse[0]);
    int Notes[CharsLength/3];
    int Timings[CharsLength/3];
    int Directions[CharsLength/3];

    int Current3Index = 0;

    for(int i = 0; i < CharsLength; i+=2)
    {
      Notes[i/3] = CharsToParse[i];
      Timings[i/3] = CharsToParse[i];
      Directions[i/3] = CharsToParse[i];
    }


    if((sizeof(Notes) && sizeof(Timings)) == sizeof(Directions))
    {
      this->Notes = Notes;
      this->Timings = Timings;
      this->Directions = Directions;
      return true;
    }
    else
    {
      return false;
    }

  }
};

