#include <Wire.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>

#include "pitches.h" //getting pitches and silence
#include "config.h" //getting some config const values as defines cus im lazy
#include "Logger.h" //Logger class with three loglevels
#include "Song.h" //da mastapiece
#include "songs.h" //songs from another file

hd44780_I2Cexp lcd(0x27, 16, 2);

Logger logger = Logger();
Song songs[1];

// NOTE_C7, 4, 2,
// NOTE_D7, 4, 2

void dispPrintAcc(String text, bool firstTime = false)
{
  if(firstTime)
  {
    lcd.clear();
    lcd.home();
    lcd.print("acc.:");
  }
  lcd.setCursor(0,1);
  lcd.print(text + "%");
}

void resetLEDs(int minpin, int maxpin)
{
  for (int i = minpin; i < maxpin; i++)
  {
    digitalWrite(i, LOW);
  }
}

float play(Song song, bool useDisplay)
{
  logger.printline("playing" + (String)song.songName);
  return play(song.songNotes, song.songTimings, song.songDirections, song.songLength, song.noteDelay, song.tactLength, song.maxPressDelay, useDisplay);
}
float play(int songNotes[], float songTimings[], int songDirections[], int songLength, int noteDelay, int tactLength, float maxPressDelay, bool useDisplay) //takes the Notes, the Timings, the Directions, the Length of the Song(IN NOTES, NOT IN MS!!!), the Length of a tact (IN MS, NOT IN NOTES!!!) and the max button press delay (Ill probably replace that with a multiplier for the absolute Length of a note).
{
  dispPrintAcc("//", true);
  logger.printline("starting song");
  float currentPressDelay = 0;
  float currentAcc = 0;
  float currentMaxPressDelay = 0;
  int pressCount = 0;
  float avgAcc = 0;

  for (int i = 0 ; i <= songLength - 1 ; i++)
  {

    int currentNote = songNotes[i]; //get current frequency
    int currentDirection = songDirections[i]; //get current direction
    float currentTiming = songTimings[i]; //get current timing

    if (tactLength * currentTiming == INFINITY || currentDirection > 2 || currentNote < 0)
    {
      logger.printline("something is wrong with the song input", "severe");
      break;
    }

    currentMaxPressDelay = (maxPressDelay * (tactLength * currentTiming));

    setLEDs(songDirections, songLength, i); //set the leds

    logger.printline((String)("playing tone with pitch, direction, Length: " + (String)(currentNote) + "," + (String)(currentDirection) + "," + (String)(tactLength * currentTiming)));

    if (currentNote == 0)
    {
      noTone(SPEAKERPIN);
    }
    else
    {
      tone(SPEAKERPIN, currentNote); //playing the current sound
    }
    if (currentDirection < 2)
    {
      currentPressDelay = measurePressDelay(currentDirection, currentMaxPressDelay);
      currentAcc = currentPressDelay / currentMaxPressDelay;
      avgAcc += currentAcc;
      pressCount++;
      logger.printline("acc.: " + (String)(100 - currentAcc * 100) + "%");
      if (useDisplay) {
        dispPrintAcc((String)(100 - currentAcc * 100));
      }
      delay(tactLength * currentTiming - currentPressDelay); //waits until the note is over, depending on the time spent in measurePressDelay
    }
    else
    {
      if (useDisplay) {
        delay(tactLength * currentTiming + 0);
      }
      else {
        delay(tactLength * currentTiming); //make each delay a bit longer to accomodate for time lost to lcd printing, deactivated it because you cant really notice it
      }
    }

    noTone(SPEAKERPIN);
    delay(tactLength / noteDelay); //adds a really smol delay after each note
  }
  noTone(SPEAKERPIN);
  resetLEDs(MINLEDPIN, MAXLEDPIN);
  logger.printline("end of song");
  avgAcc /= pressCount;
  avgAcc = 100 - 100 * avgAcc;
  return avgAcc; //returns the average accuracy
}

void setLEDs (int songDirections[], int songLength, int initialIndex) //goes through every led, turns it off, and then turns it on if it finds a matching value at the according SongDirection index
{
  for (int i = 0; i <= MAXLEDPIN_0 - MINLEDPIN_0; i++)
  {
    digitalWrite(MINLEDPIN_0 + i, LOW);
    if (songLength > initialIndex + i) //checking left notes
    {
      if (songDirections[initialIndex + i] == 0)
      {
        digitalWrite(MINLEDPIN_0 + i, HIGH);
      }
    }
  }
  for (int i = 0; i <= MAXLEDPIN_1 - MINLEDPIN_1; i++) //right notes
  {
    digitalWrite(MINLEDPIN_1 + i, LOW);
    if (songLength > initialIndex + i)
    {
      if (songDirections[initialIndex + i] == 1)
      {
        digitalWrite(MINLEDPIN_1 + i, HIGH);
      }
    }
  }
}

float measurePressDelay(int button, float maxPressDelay) //doesnt really "measure". Just waits for a specified max time and returns how long the player needed
{
  long initialMillis = millis();
  int pin;
  int wrongPin;
  switch (button)
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
  while (millis() - initialMillis <= maxPressDelay)
  {
    if (digitalRead(wrongPin) == 0)
    {
      delay(maxPressDelay - (millis() - initialMillis));
      break;
    }
    if (digitalRead(pin) == 0)
    {
      return int(millis() - initialMillis);
    }
  }
  return maxPressDelay;
}

void dispPrint(String text) {
  lcd.clear();
  lcd.home();
  lcd.print(text);
}
void dispPrint2(String text) {
  lcd.setCursor(0, 1);
  lcd.print(text);
}

bool prompt(char *prompt, String yes, String no, int noPin, int yesPin) {
  lcd.clear();
  lcd.setCursor(7, 1);
  lcd.print(no);
  lcd.setCursor(0, 1);
  lcd.print(yes);
  lcd.setCursor(0, 0);
  lcd.print(prompt);
  while (digitalRead(noPin) == HIGH && digitalRead(yesPin) == HIGH) {
    digitalRead(noPin);
    digitalRead(yesPin);
  }
  if (digitalRead(noPin) == LOW)
  {
    logger.printline((String)prompt + " false", "debug");
    return false;
  }
  else
  {
    logger.printline((String)prompt + " true", "debug");
    return true;
  }
}

void setup() //some normal setup stuff (setting pins and serial initialization)
{
  lcd.init();
  lcd.backlight();

  Song::songCount = 0;
  songs[0] = Song(noteDelay0, tactLength0, maxPressDelay0, songNotes0, songTimings0, songDirections0, sizeof(songNotes0), "Evangelion");


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
  //logger.printline((String)prompt("test", "no", "yes", BUTTONPIN_0, BUTTONPIN_1));
  //logger.printline((String)play(songs[0], true));
}

void loop()
{
  delay(750);
  for (int i = 0; i < Song::songCount; i++)
  {
    delay(750);
    if (prompt(songs[i].songName, "next", "play", BUTTONPIN_0, BUTTONPIN_1))
    {
      delay(750);
      if (prompt("mode?", "try", "play", BUTTONPIN_0, BUTTONPIN_1))
      {
        delay(500);
        float accuracy = play(songs[i], true);
        logger.printline("avg. accuracy: " + String(accuracy) + " percent");
        dispPrint("avg. acc.:");
        dispPrint2((String)accuracy + "%");
        delay(2500);
        break;
      }
      else {
        lcd.clear();
        play(songs[i], false);
        break;
      }
    }
    else
    {
      continue;
    }
  }
}
