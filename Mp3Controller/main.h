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
const int set433Pin=4;
#ifdef ON_BOARD_MOS
const int output1Pin=A0;
const int output2Pin=A1;
const int output3Pin=A2;
const int output4Pin=A3;
#if ON_BOARD_MOS>4
const int output5Pin=A4;
const int output6Pin=A5;
#if ON_BOARD_MOS>6
const int output7Pin=7;
const int output8Pin=8;
#endif
#endif
#endif

#ifdef ON_BOARD_INPUT
const int input1Pin=A0;
const int input2Pin=A1;
#if ON_BOARD_INPUT>2
const int input3Pin=A2;
#if ON_BOARD_INPUT>3
const int input4Pin=7;
const int input5Pin=8;
#if ON_BOARD_INPUT>5
const int input6Pin=9;
const int input7Pin=10;
const int input8Pin=11;
const int input9Pin=12;
const int input10Pin=13;
#endif
#endif
#endif
#endif

#ifdef ON_BOARD_BUZZ
const int buzzPin=6;
#endif
#define addr1Pin input1Pin
#define addr2Pin input2Pin
#define addr3Pin input3Pin
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
volatile unsigned long RC522ActiveMoment;
#endif
/*************************************************************************************************/


//Do not add code below this line
#endif /* _main_H_ */
