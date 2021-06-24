#include "pitches.h" //getting pitches and silence
#include "config.h" //getting some config const values as defines cus im lazy
#include "Logger.h" //Logger class with three loglevels
#include "Song.cpp" //da mastapiece

Logger logger = Logger();

int noteDelay = 512; //adds a small silent delay after each note
int tactLength = 1500; //Length of the tact (in ms)
float maxPressDelay = 1; //NOTE: must be smaller or the same as tactLength/NoteTiming (DONT FORGET TO ADD DYNAMIC DELAYS!!!) EDIT: Added dynamic delays and changed accuracy to percent
int songNotes[] = //use defined values from pitches.h (use SILENCE for silence).
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
  NOTE_A6
  
};
float songTimings[] = //assign a multiplication for the tactLength Value for every note, which is then used to determine its absolute Length, depending on the Length of one tact
{
0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25
};
int songDirections[] = //set the directions as follows: 0=left, 1=right nothing = 2.
{
  2,2,0,1,0,0,0,1,0,0,1,1,2,2,2,2,2,2,2,2,2,2,2
};

Song ACAT = Song(512,1500,1, songNotes, songTimings, songDirections, sizeof(songNotes));

// NOTE_C7, 4, 2,
// NOTE_D7, 4, 2

void resetLEDs(int minpin, int maxpin)
{
  for(int i = minpin; i < maxpin; i++)
  {
    digitalWrite(i,LOW);
  }
}

int* arrayFloatToInt(float input[], int size)
{
  int length = size / sizeof(float);
  int output[length];
  for(int i = 0; i < length; i++)
  {
    output[i] = (int)input[i];
  }
  return output;
}


float play(Song song)
{
  return play(song.songNotes, song.songTimings, song.songDirections, song.songLength, song.noteDelay, song.tactLength, song.maxPressDelay);
}
float play(int songNotes[], float songTimings[], int songDirections[], int songLength, int noteDelay, int tactLength, float maxPressDelay) //takes the Notes, the Timings, the Directions, the Length of the Song(IN NOTES, NOT IN MS!!!), the Length of a tact (IN MS, NOT IN NOTES!!!) and the max button press delay (Ill probably replace that with a multiplier for the absolute Length of a note).
{

  logger.printline("starting song");
  float currentPressDelay = 0;
  float currentAcc = 0;
  float currentMaxPressDelay = 0;
  int pressCount = 0;
  float avgAcc = 0;
  
  for (int i = 0 ; i <= songLength-1 ; i++)
  {

    int currentNote = songNotes[i]; //get current frequency
    int currentDirection = songDirections[i]; //get current direction
    float currentTiming = songTimings[i]; //get current timing

    if(tactLength * currentTiming == INFINITY || currentDirection > 2 || currentNote < 0)
    {
      logger.printline("something is wrong with the song input", "severe");
      break;
    }    

    currentMaxPressDelay = (maxPressDelay * (tactLength * currentTiming));
    
    setLEDs(songDirections, songLength, i); //set the leds
    
    logger.printline((String)("playing tone with pitch, direction, Length: " + (String)(currentNote) + "," + (String)(currentDirection) + "," + (String)(tactLength*currentTiming)));

    if(currentNote==0)
    {
      noTone(SPEAKERPIN);
    }
    else
    {
      tone(SPEAKERPIN, currentNote); //playing the current sound
    }
    if(currentDirection < 2)
    {
      currentPressDelay = measurePressDelay(currentDirection, currentMaxPressDelay);
      currentAcc = currentPressDelay / currentMaxPressDelay;
      avgAcc += currentAcc;
      pressCount++;
      delay(tactLength * currentTiming - currentPressDelay); //waits until the note is over, depending on the time spent in measurePressDelay
    }
    else
    {
      delay(tactLength * currentTiming);
    }
    
    noTone(SPEAKERPIN);
    delay(tactLength/noteDelay); //adds a really smol delay after each note
  }
  noTone(SPEAKERPIN);
  resetLEDs(MINLEDPIN, MAXLEDPIN);
  logger.printline("end of song");
  avgAcc /= pressCount;
  avgAcc = 100 - 100*avgAcc;
  return avgAcc; //returns the average accuracy
}

void setLEDs (int songDirections[], int songLength, int initialIndex) //goes through every led, turns it off, and then turns it on if it finds a matching value at the according SongDirection index
{
  for(int i = 0; i <= MAXLEDPIN_0 - MINLEDPIN_0; i++)
    {
      digitalWrite(MINLEDPIN_0 + i, LOW);
      if(songLength > initialIndex + i) //checking left notes
      {
          if(songDirections[initialIndex+i] == 0)
          {
           digitalWrite(MINLEDPIN_0 + i, HIGH);
          }
       }
    }
    for(int i = 0; i <= MAXLEDPIN_1 - MINLEDPIN_1; i++) //right notes
    {
      digitalWrite(MINLEDPIN_1 + i, LOW);
      if(songLength > initialIndex + i)
      {
          if(songDirections[initialIndex+i] == 1)
          {
           digitalWrite(MINLEDPIN_1 + i, HIGH);
          }
       }
    }
}

float measurePressDelay(int button, float maxPressDelay) //doesnt really "measure". Just waits for a specified max time and returns how long the player needed
{
  unsigned long initialMillis = millis();
  int pin;
  int wrongPin;
  switch(button)
  {
    case 0:
    pin = BUTTONPIN_0;
    wrongPin = BUTTONPIN_1;
    break;
    case 1:
    pin = BUTTONPIN_1;
    wrongPin = BUTTONPIN_0;
    break;
    default:
    logger.printline("somethings wrong i can feel it", "warning");
  }

    while(millis() - initialMillis <= maxPressDelay)
    {
      
      if(digitalRead(wrongPin) == 0)
      {
        delay(maxPressDelay - (millis() - initialMillis));
        break;
      }
      if(digitalRead(pin) == 0)
      {
       return int(millis()-initialMillis);
      }
   }
  return maxPressDelay;
}

void setup() //some normal setup stuff (setting pins and serial initialization)
{
  Serial.begin(9600);
  logger.init(&Serial);
  for (int i = BUTTONPIN_0 ; i <= BUTTONPIN_1 ; i++)
  {
      pinMode(i, INPUT_PULLUP);
      logger.printline("setting pin " + String(i) + " to input_pullup");
  }

  for (int i = MINLEDPIN ; i <= MAXLEDPIN ; i++)
  {
      pinMode(i, OUTPUT);
      logger.printline("setting pin " + String(i) + " to output");
  }
  logger.printline("avg. accuracy: " + String(play(ACAT)) + " percent"); //plays the song and prints the accuracy
}

void loop() {}
