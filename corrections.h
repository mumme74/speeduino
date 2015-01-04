/*
All functions in the gamma file return 

*/
//static byte numCorrections = 2;
#ifndef CORRECTIONS_H
#define CORRECTIONS_H

#include "globals.h"

byte correctionsTotal();
byte correctionWUE(); //Warmup enrichment
byte correctionASE(); //After Start Enrichment
byte correctionAccel(); //Acceleration Enrichment
byte correctionFloodClear(); //Check for flood clear on cranking

#endif // CORRECTIONS_H
