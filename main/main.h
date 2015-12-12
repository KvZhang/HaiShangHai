// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef _main_H_
#define _main_H_
#include "Arduino.h"
//add your includes for the project main here
#include "Config.h"
#include "EEPROM.h"
#include <avr/wdt.h>
#include "LaputaTimer.h"

#ifdef ON_BOARD_MP3
#include "LaputaMp3.h"
#endif

#ifdef ON_BOARD_RC522
#include "MFRC522.h"
#include "SPI.h"
#endif

#ifdef ON_BOARD_433MHz_SW
#include "LaputaCommSW.h"
#endif

#ifdef ON_BOARD_433MHz_HW
#include "LaputaComm.h"
#endif

//end of add your includes here
#ifdef __cplusplus
extern "C" {
#endif
void loop();
void setup();
#ifdef __cplusplus
} // extern "C"
#endif

//add your function definitions for the project main here
/************Class import**********************************************************************************/
#ifdef ON_BOARD_RC522
MFRC522 mfrc522(10,9);
MFRC522::MIFARE_Key key;
#endif

#ifdef ON_BOARD_433MHz_SW
LaputaComm myComm(ROOM_ADDRESS,ME_ADDRESS);
rxContent myCommRevContent;
#endif

#ifdef ON_BOARD_433MHz_HW
LaputaComm myComm(ROOM_ADDRESS,ME_ADDRESS);
#endif

#ifdef ON_BOARD_MP3
LaputaMp3 myMp3;
#endif

LaputaTimer myTimer;
/*************Pin define************************************************************************************/
//const int set433Pin=4;

#ifdef ON_BOARD_MOS
const volatile char outputPin[ON_BOARD_MOS]={0};
#endif

#ifdef ON_BOARD_INPUT
const volatile char inputPin[ON_BOARD_INPUT]={0};
#endif

#ifdef ON_BOARD_BUZZ
const int buzzPin=6;
#endif
/******************************Globe Flag*******************************************************************/
struct FLAG_BITES{
	unsigned char bit0:1;
	unsigned char bit1:1;
	unsigned char bit2:1;
	unsigned char bit3:1;
	unsigned char bit4:1;
	unsigned char bit5:1;
	unsigned char bit6:1;
	unsigned char bit7:1;
};
union SYS_FLAG{
	unsigned char all;
	FLAG_BITES bit;
};
volatile SYS_FLAG gFlag;
/******************************Constant Variables*******************************************************************/


/******************************Globe Variables******************************************************************/
volatile unsigned char gState=STATE_WAITING;
volatile unsigned char gTimer[TOTAL_TIMER_NUM];
#ifdef ON_BOARD_RC522
_RC522_READ_BUFFER RC522ReadBuffer;
_RC522_WRITE_BUFFER RC522WriteBuffer;
volatile unsigned char RC522ID[20];
volatile unsigned char RC522LastID;
volatile unsigned long RC522ActiveMoment;//this card active moment
#endif
volatile unsigned int gameStartMoment;// start time in seconds
volatile unsigned char totalPlayerNum;//
/*************************************************************************************************/


//Do not add code below this line
#endif /* _main_H_ */
