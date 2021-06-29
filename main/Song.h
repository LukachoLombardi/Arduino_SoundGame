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
float debug = 0;

enum SongDataType {Delay, TactLength, MaxPressDelay, Notes, Timings, Directions, Length};

Song();
Song(int noteDelay, int TactLength, float maxPressDelay, int *songNotes, float *songTimings, int *songDirections, int songSize);

void setSongData(SongDataType type, int data);
void setSongData(SongDataType type, int *data);
void setSongData(SongDataType type, float data);
void setSongData(SongDataType type, float *data);

};
