#include "pitches.h" //getting pitches and silence
#include "config.h" //getting some config const values as defines cus im lazy

int NoteDelay = 512; //adds a small silent delay after each note
int TactLength = 1500; //Length of the tact (in ms)
float MaxPressDelay = 1; //NOTE: must be smaller or the same as TactLength/NoteTiming (DONT FORGET TO ADD DYNAMIC DELAYS!!!) EDIT: Added dynamic delays and changed accuracy to percent
int SongNotes[] = //use defined values from pitches.h (use SILENCE for silence).
{
  NOTE_A5,
  NOTE_C6,
  NOTE_D6,
  NOTE_C6,
  NOTE_D6,
  NOTE_D6,
  NOTE_D6,
  NOTE_G6,
  NOTE_F6,
  NOTE_E6,
  NOTE_D6,
  NOTE_E6,
  NOTE_E6,
  NOTE_G6,
  NOTE_A6,
  NOTE_D6,
  NOTE_C6,
  NOTE_G6,
  NOTE_G6,
  NOTE_E6,
  NOTE_G6,
  NOTE_G6,
  NOTE_A6,
  
};
float SongTimings[] = //assign a denominator for the TactLength Value for every note, which is then used to determine its absolute Length, depending on the Length of one tact
{
  4,4,4.0/3.0,4.0/3.0,8,8,8,8,8,16,8,4.0/3.0,4,4,4.0/3.0,4.0/3.0,8,8,8,8,8,4.0/3.0,4.0/3.0
};
int SongDirections[] = //set the directions as follows: 0=left, 1=right, and basically everything else for nothing but please use nothing = 2.
{
  0,0,0,1,0,0,0,1,0,0,1,1
};

// NOTE_C7, 4, 2,
// NOTE_D7, 4, 2
template <typename T> //function to display serial output after initialization with a shortened syntax and two importance levels, which can be toggled in the config
void println(T in, bool important = false)
{
 if(!ONLYIMPORTANTOUTPUT)
 {
  Serial.println(in);
 }
 else
 {
  if (important)
  {
   Serial.println(in);
  }
 }
}

void resetLEDs(int minpin, int maxpin)
{
  for(int i = minpin; i < maxpin; i++)
  {
    digitalWrite(i,LOW);
  }
}

int Play(int SongNotes[], int SongTimings[], int SongDirections[], int SongLength, int NoteDelay, int TactLength, float MaxPressDelay) //takes the Notes, the Timings, the Directions, the Length of the Song(IN NOTES, NOT IN MS!!!), the Length of a tact (IN MS, NOT IN NOTES!!!) and the max button press delay (Ill probably replace that with a multiplier for the absolute Length of a note).
{

  println("starting song", true);
  int currentPressDelay = 0;
  float currentAcc = 0;
  int currentMaxPressDelay = 0;
  int pressCount = 0;
  float avgAcc = 0;
  
  for (int i = 0 ; i <= SongLength-1 ; i++)
  {
    int CurrentFreq = SongNotes[i]; //get current frequency
    int CurrentDir = SongDirections[i]; //get current direction
    int CurrentTiming = SongTimings[i]; //get current timing
    
    setLEDs(SongDirections, SongLength, i); //set the leds
    
    println("playing tone with pitch, direction, Length: " + String(CurrentFreq) + "," + String(CurrentDir) + "," + String(TactLength/CurrentTiming));

    if(CurrentFreq==0)
    {
      noTone(SPEAKERPIN);
    }
    else
    {
      tone(SPEAKERPIN, CurrentFreq); //playing the current sound
    }

    if(CurrentDir == 0 || CurrentDir == 1)
    {
      currentMaxPressDelay = (int)(MaxPressDelay * (TactLength / CurrentTiming));
      currentPressDelay = measurePressDelay(CurrentDir, currentMaxPressDelay);
      currentAcc = currentPressDelay / currentMaxPressDelay;
      avgAcc += currentAcc;
      pressCount++;
    }

    delay(TactLength / CurrentTiming - currentPressDelay); //waits until the note is over, depending on the time spent in measurePressDelay
    noTone(SPEAKERPIN);
    delay(TactLength/NoteDelay); //adds a really smol delay after each note
  }
  noTone(SPEAKERPIN);
  resetLEDs(MINLEDPIN,MAXLEDPIN);
  println("end of song", true);
  avgAcc /= pressCount;
  avgAcc = 100 - 100*avgAcc;
  return avgAcc; //returns the average accuracy
}

void setLEDs (int SongDirections[], int SongLength, int CurrentIndex) //goes through every led, turns it off, and then turns it on if it finds a matching value at the according SongDirection index
{
  for(int i = 0; i <= MAXLEDPIN_0 - MINLEDPIN_0; i++)
    {
      digitalWrite(MINLEDPIN_0 + i, LOW);
      if(SongLength > CurrentIndex + i) //checking left notes
      {
          if(SongDirections[CurrentIndex+i] == 0)
          {
           digitalWrite(MINLEDPIN_0 + i, HIGH);
          }
       }
    }
    for(int i = 0; i <= MAXLEDPIN_1 - MINLEDPIN_1; i++) //right notes
    {
      digitalWrite(MINLEDPIN_1 + i, LOW);
      if(SongLength > CurrentIndex + i)
      {
          if(SongDirections[CurrentIndex+i] == 1)
          {
           digitalWrite(MINLEDPIN_1 + i, HIGH);
          }
       }
    }
}

int measurePressDelay(int button, int MaxPressDelay) //doesnt really "measure". Just waits for a specified max time and returns how long the player needed
{
  unsigned long InitialMillis = millis();
  int pin;
  int WrongPin;
  switch(button)
  {
    case 0:
    pin = BUTTONPIN_0;
    WrongPin = BUTTONPIN_1;
    break;
    case 1:
    pin = BUTTONPIN_1;
    WrongPin = BUTTONPIN_0;
    break;
    default:
    println("somethings wrong i can feel it", true);
  }

    while(millis() - InitialMillis <= MaxPressDelay)
    {
      
      if(digitalRead(WrongPin) == 0)
      {
        delay(MaxPressDelay - (millis() - InitialMillis));
        break;
      }
      if(digitalRead(pin) == 0)
      {
       return int(millis()-InitialMillis);
      }
   }
  return MaxPressDelay;
}


void setup() //some normal setup stuff (setting pins and serial initialization)
{
  Serial.begin(9600);
  println("Serial Initialized", true);

  for (int i = BUTTONPIN_0 ; i <= BUTTONPIN_1 ; i++)
  {
      pinMode(i, INPUT_PULLUP);
      println("setting pin " + String(i) + " to input_pullup");
  }

  for (int i = MINLEDPIN ; i <= MAXLEDPIN ; i++)
  {
      pinMode(i, OUTPUT);
      println("setting pin " + String(i) + " to output");
  }
  println("avg. accuracy: " + String(Play(SongNotes, SongTimings, SongDirections,  sizeof(SongNotes)/sizeof(SongNotes[0]), NoteDelay, TactLength, MaxPressDelay)) + " percent", true); //plays the song and prints the accuracy
}

void loop() //probably not needed here
{

}
