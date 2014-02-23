// Nur nach Hause
const int bpm=90;
const int takt=4;
const float visualizer_pitch=0.45;
const int pitchOctave=0;
const prog_char track1[] PROGMEM = 
"d3.e3."
"g3...g3.......h3.d4."
"e4...e4.......e4.d4."
"d4...c4.......c4.h3."
"h3..........-d4.h3."

"h3...a3.......h3.a3-"
"g3...e3.......e3.a3-"
"h3...a3.......a3.g3."
"g3.......----d3.e3."
"fis3......."
;
PROGMEM const char * const tracks[TRACKS] = { track1,track1 };

