/*
 * Config.h
 *
 *  Created on: 2015Äê11ÔÂ5ÈÕ
 *      Author: zhangyu
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include "Room__define.h"
/*******Peripheral******************************************************************************/
#ifndef _PERIPHERALS
#define _PERIPHERALS
//|MOS1|MOS2|MOS3|MOS4|MOS5|MOS6|MOS7|MOS8|
//-----|----|----|----|----|----|----|----|
//| A0 | A1 | A2 | A3 | A4 | A5 | D7 | D8 |
#define ON_BOARD_MOS		6	//1~4,5~6,7~8 three options
//|Input1|Input2|Input3|Input4|Input5|Input6|Input7|Input8|Input9|Input10|
//-------|------|------|------|------|------|------|------|------|-------|
//|  D4  |  D5  |  D6  |  D7  |  D8  |  D9  | D10  | D11  | D12  |  D13  |
#define ON_BOARD_INPUT		2	//1~2,3,4~5,6~10 four options
//|RESET|SELECT|
//|-----|------|
//| D9  |  D10 |
#define ON_BOARD_RC522
//#define ON_BOARD_MP3
//BUZZ_PIN->D6
#define ON_BOARD_BUZZ
//|SW_RX|SW_TX|
//|-----|-----|
//| D2  | D3  |
#define ON_BOARD_433MHz_SW
//#define ON_BOARD_433MHz_HW

//#define ON_BOARD_IIC
#endif
/*******Comm address******************************************************************************/
#define ROOM_ADDRESS 	(0x01)
#define ME_ADDRESS		(0x01)
/********Other define**********************************************************************************/
#define BUZZ_ON 		LOW
#define BUZZ_OFF		HIGH
#define MOS_INIT_STATE		0
#define MOS_ACTIVE_STATE	1
/********ENUM************************************************************************************************/
typedef enum {
	STATE_WAITING, STATE_ACTING, STATE_RECOVERING, STATE_RETRY,
} _ME_STATE;
//Timer type
typedef enum{
	TIMER1,
	TOTAL_TIMER_NUM,
} _SYS_TIMER;
#endif /* CONFIG_H_ */
