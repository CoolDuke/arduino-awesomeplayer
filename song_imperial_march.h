// Star Wars - Imperial March
const int bpm=90;
const int takt=4;
const float visualizer_pitch=0.45;
const int pitchOctave=2;
const prog_char track1[] PROGMEM = 
"g2..-g2..-g2..-es2.-ais2"
"g2..-es2.-ais2g2......-"
"d3..-d3..-d3..-es3.-ais2"

"g2..-es2.-ais2g2..-----"
"g3..-g2.-g2g3..-f3.-f3"
"es3d3es3.--g2-c3..-c3-b2-"

"b2a2b2.--es2-g2..-es2.-b2"
"g2..-es2.-b2g2......-"
"g3..-g2.-g2g3..-f3.-f3"

"es3d3es3.--g2-c3..-c3-b2-"
"b2a2b2.--es2-g2..-es2.-d3"
"g2..-es2.-b2g2......-"
;

const prog_char track2[] PROGMEM = 
"g1..-g1..-g1..-es1..-"
"g1..-es1..-g1-g1g1g1-g1-"
"g1..-g1..-g1..-ges1..-"

"es1..-c1..-g1-g1g1es2-f2-"
"g2..-g1..-g1..-g1..-"
"cis1..-c1..-c1..-c1..-"

"es1..-es1..-es1..-es1..-"
"g1..-g1..-g1..-g1-g1g1"
"es1..-es1..-g1..-g1..-"

"cis1..-c1..-c1..-c1..-"
"es1..-es1..-es1..-cis1..-"
"g1..-es1..-g1..-g0..-"
;

PROGMEM const char * const tracks[TRACKS] = { track1,track2 };

