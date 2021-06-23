class Song
{

public: 

int noteDelay = 512; //adds a small silent delay after each note
int tactLength = 1500; //Length of the tact (in ms)
float maxPressDelay = 1; //NOTE: must be smaller or the same as TactLength/NoteTiming (DONT FORGET TO ADD DYNAMIC DELAYS!!!) EDIT: Added dynamic delays and changed accuracy to percent
int *songNotes;
float *songTimings;
int *songDirections;
int songLength;

enum SongDataType {Delay, TactLength, MaxPressDelay, Notes, Timings, Directions, Length};

Song(int noteDelay, int TactLength, float maxPressDelay, int songNotes[], float songTimings[], int songDirections[], int songLength)
{
  setSongData(Song::Delay, noteDelay);
  setSongData(Song::TactLength, TactLength);
  setSongData(Song::MaxPressDelay, maxPressDelay);
  setSongData(Song::Notes, songNotes);
  setSongData(Song::Timings, songTimings);
  setSongData(Song::Directions, songDirections);
  songLength = songLength;
}

void setSongData(SongDataType type, int data)
{
  switch(type)
  {
    case Delay:
      noteDelay = data;
      break;
    case TactLength:
      tactLength = data;
      break;
  }
}
void setSongData(SongDataType type, int data[])
{
  switch(type)
  {
    case Notes:
      delete [] this->songNotes;
      this->songNotes = new int[songLength];
      this->songNotes = data;
      break;
    case Directions:
      delete [] this->songDirections;
      this->songDirections = new int[songLength];
      this->songDirections = data;
      break;
  }
}
void setSongData(SongDataType type, float data)
{
  switch(type)
  {
    case MaxPressDelay:
      maxPressDelay = data;
      break;
  }
}
void setSongData(SongDataType type, float data[])
{
  switch(type)
  {
    case Timings:
      delete [] this->songTimings;
      this->songTimings = new float[songLength];
      this->songTimings = data;
      break;
  }
}

};