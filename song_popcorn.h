// popcorn
const int bpm=135;
const int takt=8;
const float visualizer_pitch=1.1;
const int pitchOctave=0;
const prog_char track1[] PROGMEM =
"c5---ais4---c5---g4---dis4-g4---c4-------"
"c5---ais4---c5---g4---dis4-g4---c4-------"
"c5---d5-dis5---d5---d5-"
"dis5---c5-d5---c5.--c5-"
"d5---ais4-c5---ais4---ais4-"
"c5---gis4---c5-------"
"c6---ais5---c6---g5---dis5-g5---c5-------"
"c6---ais5---c6---g5---dis5-g5---c5-------"
"c6---d6-dis6---d6---d6-"
"dis6---c6-d6---c6.--c6-"
"d6---ais5-c6---ais5---ais5-"
"c6---gis5.--c6---c5..-";
PROGMEM const char * const tracks[TRACKS] = { track1,track1 };

