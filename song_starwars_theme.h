// Star Wars - Main Theme
const int bpm=90;
const int takt=4;
const float visualizer_pitch=0.45;
const int pitchOctave=1;
const prog_char track1[] PROGMEM = 
"d2d2d2g2.....d3.....c3b2a2g3.....d3..c3b2a2g3.....d3..c3b2c3a2.....d2..g2.....d3....."
"c3b2a2g3.....d3..c3b2a2g3.....d3..c3b2c3a2.....d2..e2...c3b2a2g2g2a2b2a2.e2fis2d2.."
"e2...c3b2a2g2d3.a2......d2..e2...c2b2a2g2"
"g2a2b2a2.e2fis2.d3..g3fis3es3d3c3ais2a2g2d3......d2..";
PROGMEM const char * const tracks[TRACKS] = { track1,track1 };

