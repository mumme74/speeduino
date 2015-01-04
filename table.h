/*
This file is used for everything related to maps/tables including their definition, functions etc
*/
#ifndef TABLE_H
#define TABLE_H

#include <Arduino.h>
#include "globals.h"

/*
The 2D table can contain either 8-bit (byte) or 16-bit (int) values
The valueSize variable should be set to either 8 or 16 to indicate this BEFORE the table is used
*/
struct table2D {
  byte valueSize; 
  byte xSize;
  
  byte *values;
  byte *axisX;
  
  int *values16;
  int *axisX16;
};

void table2D_setSize(struct table2D targetTable, byte newSize);

struct table3D {
  //All tables must be the same size for simplicity
  const static byte xSize = 8;
  const static byte ySize = 8;
  
  byte values[ySize][xSize];
  int axisX[xSize];
  int axisY[ySize];
  
};

/*
3D Tables have an origin (0,0) in the top left hand corner. Vertical axis is expressed first.
Eg: 2x2 table
-----
|2 7|
|1 4|
-----

(0,1) = 7
(0,0) = 2
(1,0) = 1

*/
int get3DTableValue(struct table3D, int, int);
int table2D_getValue(struct table2D, int);

// declared in speeduino.ino
extern struct table3D fuelTable; //8x8 fuel map
extern struct table3D ignitionTable; //8x8 ignition map
extern struct table2D taeTable; //4 bin TPS Acceleration Enrichment map (2D)
extern struct table2D WUETable; //10 bin Warm Up Enrichment map (2D)

extern byte cltCalibrationTable[CALIBRATION_TABLE_SIZE];
extern byte iatCalibrationTable[CALIBRATION_TABLE_SIZE];
extern byte o2CalibrationTable[CALIBRATION_TABLE_SIZE];

#endif // TABLE_H

