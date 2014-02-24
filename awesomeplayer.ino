/* AwesomePlayer for Arduino */

#include <Tone.h>
#include <avr/pgmspace.h>

//how many songs, speakers and leds are we using
#define SONGCOUNT 6
#define TRACKS 2
#define LEDCOUNT 6

//first song
byte currentSong=0;

//define some information for each song
const int bpm[SONGCOUNT]               ={ 135,   90,   90,   90,   90,  120};
const int takt[SONGCOUNT]              ={   8,    4,    4,    4,    4,    4};
const float visualizer_pitch[SONGCOUNT]={1.10, 0.45,    1, 0.40,    1,    1};
const int pitchOctave[SONGCOUNT]       ={   0,    1,    2,    0,    0,    1};

//create speaker object
Tone spkr[TRACKS];

//this is where we store the track position
unsigned int flashAddress[TRACKS];

//define arduino ports for speakers, leds and buttons
int speakers[TRACKS]={9,11};
int leds[LEDCOUNT]={2,3,4,5,6,7};
int muteButtonPort=13;
int nextButtonPort=12;

//load song data
#include "song_popcorn.h"
#include "song_starwars_theme.h"
#include "song_imperial_march.h"
#include "song_nur_nach_hause.h"
#include "song_monkey_island_theme.h"
#include "song_back_to_the_future.h"

//calculate visualizer frequencies
int visualizer_steps=1100/LEDCOUNT*visualizer_pitch[currentSong];

//declare some more variables
boolean mute=0;
boolean blinkLeds=0;

//returns the start address of a track of the current song
int getTrackAddress(int track){
  int address;
  switch(currentSong){
    case 0:
      address=pgm_read_word(&song0_tracks[track]);  
      break;     
    case 1:
      address=pgm_read_word(&song1_tracks[track]);  
      break;     
    case 2:
      address=pgm_read_word(&song2_tracks[track]);  
      break;     
    case 3:
      address=pgm_read_word(&song3_tracks[track]);  
      break;     
    case 4:
      address=pgm_read_word(&song4_tracks[track]);  
      break;     
    case 5:
      address=pgm_read_word(&song5_tracks[track]);  
      break;     
  }
  return address; 
}

//setup configuration
void setup(){
  Serial.begin(9600);
  
  //initialize buttons
  pinMode(muteButtonPort,INPUT);
  pinMode(nextButtonPort,INPUT);
  
  //initialize speakers
  for(int i=0;i<TRACKS;i++)
    spkr[i].begin(speakers[i]);

  //initialize leds and animate them
  for(int i=0;i<LEDCOUNT;i++){
    pinMode(leds[i],OUTPUT);
    toggle_led(leds[i]);
    delay(100);
  }
  for(int i=LEDCOUNT-1;i>=0;i--){
    toggle_led(leds[i]);
    delay(100);
  }
  toggle_led(0);

  //get start address of all tracks of first song
  for(int t=0;t<TRACKS;++t)
    flashAddress[t]=getTrackAddress(t);  
}

//main loop
void loop(){
  
  //toggle mute if mute button is being pushed
  if(digitalRead(muteButtonPort)){
    delay(200);
    mute^=1;
  }
  
  //selects the next song if next button is beeing pushed
  if(digitalRead(nextButtonPort)){
    delay(200);
    
    //select first song if last one is active, otherwise select the next one
    if(currentSong==SONGCOUNT-1) currentSong=0;
    else currentSong++;
    
    //get memory addresses of new song
    for(int t=0;t<TRACKS;++t)
      flashAddress[t]=getTrackAddress(t);  
    
    //calculate new visualizer frequencies 
    visualizer_steps=1100/LEDCOUNT*visualizer_pitch[currentSong];
  }
   
  //if muted
  if(mute){
    //stop all tones
    for(int t=0;t<TRACKS;++t)
      spkr[t].stop();
    
    //let leds blink
    blinkLeds^=1;
    for(int i=0;i<LEDCOUNT;i++)
      digitalWrite(leds[i],blinkLeds);  
    
    delay(500);
    
    //stop here
    return;
  }
  boolean trackFinished=0;
    
  //play a note on every track
  for(int t=0;t<TRACKS;++t){
    unsigned int lastAddress[TRACKS];

   //read next note for current track
    int note=getNextNote(t);
    switch(note) {
      case -2: //hold tone - do nothing
        break;
      case -1: //track is finished - reset memory address
        trackFinished = 1;
        flashAddress[t]=getTrackAddress(t);
        break;
      case 0: //pause - stop tone and led
        spkr[t].stop();
        if(t==0) toggle_led(0);
        break;
      default: //every other number is treated as frequency
        spkr[t].stop();
        delay(1);
        spkr[t].play(note);
        //only visualize primary channel
        if(t==0){
          led_visualizer(note);
          //Serial.println(note);
        }
    }
  }
  //insert pause defined by bpm and takt
  delay(60000/bpm[currentSong]/takt[currentSong]);
}

//reads the next note from the flash memory and returns its frequency
int getNextNote(int t) {
  String thisNote;
  float thisLength;
  float pauseLength;
  int thisOctave;
    
  char note=0;

  //read note from flash
  note=(char) pgm_read_byte(flashAddress[t]++);
  if(note=='\0') return -1;       //end of track
  else if(note == '.') return -2; //hold note
  else if(note == '-') return 0;  //insert pause
  
  //a note may have more than one letter, so we need a loop to read it
  thisNote="";
  while (note != '.' && note != '-' && !isANumber(note)) {
    thisNote += note;
    note=(char) pgm_read_byte(flashAddress[t]++);
  }

  //next char has to be the octave value - read it
  thisOctave = 0;
  char thisOctaveStr = note;
  thisOctave = atoi(&thisOctaveStr);

  //get freqency of note and octave and return it
  return noteNameToFreq(thisNote,thisOctave);
}

//returns the frequency of a given note and octave
int noteNameToFreq(String note, int octave) {
  //change octave if a song needs it
  octave+=pitchOctave[currentSong];

  //get frequencies by note name
  float val = 0.0;
  if(note.equalsIgnoreCase("C"))
    val= NOTE_C3; 
  else if(note.equalsIgnoreCase("CIS") || note.equalsIgnoreCase("DES"))
    val= NOTE_CS3; 
  else if(note.equalsIgnoreCase("D"))
    val= NOTE_D3; 
  else if(note.equalsIgnoreCase("DIS") || note.equalsIgnoreCase("ES"))
    val= NOTE_DS3; 
  else if(note.equalsIgnoreCase("E") || note.equalsIgnoreCase("FES"))
    val= NOTE_E3; 
  else if(note.equalsIgnoreCase("F"))
    val= NOTE_F3; 
  else if(note.equalsIgnoreCase("FIS") || note.equalsIgnoreCase("GES"))
    val= NOTE_FS3; 
  else if(note.equalsIgnoreCase("G"))
    val= NOTE_G3; 
  else if(note.equalsIgnoreCase("GIS") || note.equalsIgnoreCase("AES"))
    val= NOTE_GS3; 
  else if(note.equalsIgnoreCase("A"))
    val= NOTE_A3; 
  else if(note.equalsIgnoreCase("AIS"))
    val= NOTE_AS3; 
  else if(note.equalsIgnoreCase("H") || note.equalsIgnoreCase("B"))
    val= NOTE_B3; 
  
  //add octave to note frequency
  if(octave > 3) {
    while(3-(octave--)) {
      val*= 2;
    }
  } else {
    while(3-(octave++)) {
      val/= 2;
    }
  }
  
  //return the note frequency
  return val;
}

//true if given character is a number
boolean isANumber(char c) {
  return (c >= '1' && c <= '9') || c == '0';
}

//turns the given led on and turns off the others
void toggle_led(int led){
  if(led>0) digitalWrite(led,HIGH);
  for(int i=0;i<LEDCOUNT;i++)
    if(led!=leds[i]) 
      digitalWrite(leds[i],LOW);  
}

//devides frequency spectrum into peaces and flashes the leds according to it
void led_visualizer(int freq){
  for(int i=0;i<=LEDCOUNT;i++){
    if(freq<(i+1)*visualizer_steps){
       toggle_led(leds[i]);
       break;
    }
  }  
}
