#include <Tone.h>

//how many speakers and leds are we using
#define TRACKS 2
#define LEDCOUNT 6

//create speaker object
Tone spkr[TRACKS];

//this is where we store the track position
unsigned int flashAddress[TRACKS];

//define arduino ports for speakers, leds and mute button
int speakers[TRACKS]={9,11};
int leds[LEDCOUNT]={2,3,4,5,6,7};
int muteButtonPort=13;

//select a song by exclusively uncommenting it

#include "song_popcorn.h"
//#include "song_starwars_theme.h"
//#include "song_imperial_march.h"
//#include "song_nur_nach_hause.h"
//#include "song_monkey_island_theme.h"
//#include "song_back_to_the_future.h"

//#include "nosong.h"

//calculate visualizer frequencies
int visualizer_steps=1100/LEDCOUNT*visualizer_pitch;

void setup(){
  Serial.begin(9600);
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

  //get start address of all tracks
  for(int t=0;t<TRACKS;++t)
    flashAddress[t]=pgm_read_word(&tracks[t]);
    
}

boolean mute=0;
boolean blinkLeds=0;

void loop(){
  
  //toggle mute if mute button is being pushed
  if(digitalRead(muteButtonPort)){
    delay(200);
    mute^=1;
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
        flashAddress[t]=pgm_read_word(&tracks[t]);
        break;
      case 0: //pause - stop tone
        spkr[t].stop();
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
  delay(60000/bpm/takt);
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
  octave+=pitchOctave;

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
