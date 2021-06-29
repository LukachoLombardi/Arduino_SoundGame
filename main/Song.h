#ifndef Song_H
#define Song_H

class Song
{
public: 
static int songCount;
char *songName;

int noteDelay = 512; //adds a small silent delay after each note
int tactLength = 1500; //Length of the tact (in ms)
float maxPressDelay = 1; //NOTE: must be smaller or the same as TactLength/NoteTiming (DONT FORGET TO ADD DYNAMIC DELAYS!!!) EDIT: Added dynamic delays and changed accuracy to percent
int *songNotes;
float *songTimings;
int *songDirections;
int songLength;

enum SongDataType {Delay, TactLength, MaxPressDelay, Notes, Timings, Directions, Length};

Song();
Song(int noteDelay, int TactLength, float maxPressDelay, int *songNotes, float *songTimings, int *songDirections, int songSize, char name[16]);

void setSongData(SongDataType type, int data);
void setSongData(SongDataType type, int *data);
void setSongData(SongDataType type, float data);
void setSongData(SongDataType type, float *data);

};
#endif
