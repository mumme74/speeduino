#ifndef COMMS_H
#define COMMS_H
#include "globals.h"
#include "utils.h"
#include "scheduler.h"
#include "storage.h"

#define vePage    1
#define ignPage   2

byte currentPage;

void command();
void sendValues(int length);
void receiveValue(byte offset, byte newValue);
void saveConfig();
void sendPage();
void receiveCalibration(byte tableID);
void sendToothLog(bool useChar);
void testComm();

#endif // COMMS_H
