// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef _testHWReset_H_
#define _testHWReset_H_
#include "Arduino.h"
//add your includes for the project testHWReset here
#include <avr/wdt.h>


//end of add your includes here
#ifdef __cplusplus
extern "C" {
#endif
void loop();
void setup();
#ifdef __cplusplus
} // extern "C"
#endif

//add your function definitions for the project testHWReset here

static unsigned char i=0;


//Do not add code below this line
#endif /* _testHWReset_H_ */
