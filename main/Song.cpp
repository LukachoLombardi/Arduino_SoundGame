#include "Song.h"
#include "Arduino.h"
int Song::songCount = 0;

Song::Song(){}
Song::Song(int noteDelay, int TactLength, float maxPressDelay, int *songNotes, float *songTimings, int *songDirections, int songSize, String name)
{
  setSongData(Song::Delay, noteDelay);
  setSongData(Song::TactLength, TactLength);
  setSongData(Song::MaxPressDelay, maxPressDelay);
  setSongData(Song::Notes, songNotes);
  setSongData(Song::Timings, songTimings);
  setSongData(Song::Directions, songDirections);
  setSongData(Song::Length, int(songSize / sizeof(int)));

  this->name = name;
  Song::songCount++;
}

void Song::setSongData(SongDataType type, int data)
{
  switch(type)
  {
    case Delay:
      noteDelay = data;
      break;
    case TactLength:
      tactLength = data;
      break;
    case Length:
      songLength = data;
      break;      
  }

}
void Song::setSongData(SongDataType type, int *data)
{
  switch(type)
  {
    case Notes:
      delete [] songNotes;
      songNotes = new int[songLength];
      songNotes = data;
      break;
    case Directions:
      delete [] songDirections;
      songDirections = new int[songLength];
      songDirections = data;
      break;      
  }
}
void Song::setSongData(SongDataType type, float data)
{
  switch(type)
  {
    case MaxPressDelay:
      maxPressDelay = data;
      break;   
  }
}
void Song::setSongData(SongDataType type, float *data)
{
  switch(type)
  {
    case Timings:
      delete [] songTimings;
      songTimings = new float[songLength];
      songTimings = data;
      break;      
  }
}