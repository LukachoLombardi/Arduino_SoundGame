int noteDelay0 = 512; //adds a small silent delay after each note
int tactLength0 = 1500; //Length of the tact (in ms)
float maxPressDelay0 = 1; //NOTE: must be smaller or the same as tactLength/NoteTiming (DONT FORGET TO ADD DYNAMIC DELAYS!!!) EDIT: Added dynamic delays and changed accuracy to percent
int songNotes0[] = //use defined values from pitches.h (use SILENCE for silence).
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
float songTimings0[] = //assign a multiplication for the tactLength Value for every note, which is then used to determine its absolute Length, depending on the Length of one tact
{
0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25
};
int songDirections0[] = //set the directions as follows: 0=left, 1=right nothing = 2.
{
  2,2,0,1,0,0,0,1,0,0,1,1,2,2,2,2,2,2,2,2,2,2,2
};
