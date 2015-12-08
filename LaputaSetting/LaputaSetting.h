// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef _LaputaSetting_H_
#define _LaputaSetting_H_
#include "Arduino.h"
//add your includes for the project LaputaSetting here
#include "MFRC522.h"
#include "SoftwareSerial.h"
#include "LaputaCommSW.h"
//end of add your includes here
#ifdef __cplusplus
extern "C" {
#endif
void loop();
void setup();
#ifdef __cplusplus
} // extern "C"
#endif

//add your function definitions for the project LaputaSetting here
const int setPin=5;
LaputaComm myComm(1,0);
typedef struct{
	unsigned char objTheme;
	unsigned char objAddr;
}_SERIAL_BUFFER;
volatile _SERIAL_BUFFER myCommBuffer;
volatile long settingBRD=9600;
volatile long commBRD=19200;

//Do not add code below this line
#endif /* _LaputaSetting_H_ */
