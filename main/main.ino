#include "pitches.h" //getting pitches and silence
#include "config.h" //getting some config const values as defines cus im lazy
#include "Song.cpp" //da mastapiece

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
  NOTE_A6,
  
};
float songTimings[] = //assign a denominator for the tactLength Value for every note, which is then used to determine its absolute Length, depending on the Length of one tact
{
  4,4,4,4,8,8,8,8,8,16,8,4,4,4,4,4,8,8,8,8,8,4,4,4.0/3.0
};
int songDirections[] = //set the directions as follows: 0=left, 1=right, and basically everything else for nothing but please use nothing = 2.
{
  0,0,0,1,0,0,0,1,0,0,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,8
};

Song test = Song(512,1500,1, {880}, {1}, {0}, 1);

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

int arrayFloatToInt(float input[], int size)
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
  play(song.songNotes, song.songTimings, song.songDirections, song.songLength, song.noteDelay, song.tactLength, song.maxPressDelay);
}
float play(int songNotes[], float songTimings[], int songDirections[], int songLength, int noteDelay, int tactLength, float maxPressDelay) //takes the Notes, the Timings, the Directions, the Length of the Song(IN NOTES, NOT IN MS!!!), the Length of a tact (IN MS, NOT IN NOTES!!!) and the max button press delay (Ill probably replace that with a multiplier for the absolute Length of a note).
{

  println("starting song", true);
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
    
    currentMaxPressDelay = (maxPressDelay * (tactLength / currentTiming));
    
    setLEDs(songDirections, songLength, i); //set the leds
    
    println("playing tone with pitch, direction, Length: " + String(currentNote) + "," + String(currentDirection) + "," + String(tactLength/currentTiming));

    if(currentNote==0)
    {
      noTone(SPEAKERPIN);
    }
    else
    {
      tone(SPEAKERPIN, currentNote); //playing the current sound
    }

    if(currentDirection == 0 || currentDirection == 1)
    {
      
      currentPressDelay = measurePressDelay(currentDirection, currentMaxPressDelay);
      currentAcc = currentPressDelay / currentMaxPressDelay;
      avgAcc += currentAcc;
      pressCount++;
      delay(tactLength / currentTiming - currentPressDelay); //waits until the note is over, depending on the time spent in measurePressDelay
    }
    else
    {
      delay(tactLength / currentTiming);
    }
    
    noTone(SPEAKERPIN);
    delay(tactLength/noteDelay); //adds a really smol delay after each note
  }
  noTone(SPEAKERPIN);
  resetLEDs(MINLEDPIN, MAXLEDPIN);
  println("end of song", true);
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
    println("somethings wrong i can feel it", true);
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
  //println("avg. accuracy: " + String(play(songNotes, songTimings, songDirections,  sizeof(songNotes)/sizeof(songNotes[0]), noteDelay, tactLength, maxPressDelay)) + " percent", true); //plays the song and prints the accuracy
  println(test.songNotes[0]);
  Serial.println(play(test));
}

void loop() {}
