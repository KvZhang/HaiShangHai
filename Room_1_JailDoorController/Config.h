/*
 * Config.h
 *
 *  Created on: 2015年11月5日
 *      Author: zhangyu
 */

#ifndef CONFIG_H_
#define CONFIG_H_

/*******Peripheral******************************************************************************/
#define ON_BOARD_MOS		1	//1~4,5~6,7~8 three options
#define MOS_INIT_STATE		0
#define MOS_ACTIVE_STATE	1

#define ON_BOARD_INPUT		3	//1~2,3,4~5,6~10 four options
#define ON_BOARD_RC522

//#define ON_BOARD_MP3
#define ON_BOARD_BUZZ

//#define ON_BOARD_433MHz_SW
//#define ON_BOARD_433MHz_HW

//#define ON_BOARD_IIC
/*******Comm address******************************************************************************/
#define ROOM_ADDRESS 	(0x01)
#define ME_ADDRESS		(0x01)
typedef enum{
	DEVICE1_ADDR=1,
	TOTOL_DEVICE_NUMBER,
};
typedef enum {
	COMM_RESET_ME = 0x20,
	COMM_TOTLE_PERSON_NUM,
} _COMM_CMD_TYPE;
/********Other define**********************************************************************************/
#define BUZZ_ON 		1
#define BUZZ_OFF		0
#define DOOR_OPEN		1
#define DOOR_CLOSE		0
#define OPEN_DOOR_LEN	(30000)
#define DEAD_TIME		(60000)
/********ENUM************************************************************************************************/
typedef enum {
	STATE_WAITING, STATE_ACTING, STATE_RECOVERING, STATE_RETRY,
} _ME_STATE;

//Uart with nodeMCU
typedef enum{
	NODEMCU_ACK = ('K'),
	NODEMCU_REST = ('R'),
	NODEMCU_OPEN = ('O'),
	NODEMCU_QUESTION = ('Q'),
} _NODE_MCU_CMD;
typedef enum{
	OPEN_DOOR_TIMER,
	DEAD_TIMER,
	TOTAL_TIMER_NUM,
} _SYS_TIMER;

typedef enum{
	READ_BLOCK=5,
	WRITE_BLOCK=6,
} _RC522_BLOCK;
typedef union{
	struct{
		unsigned char cardNumber;
		unsigned char cardType;
		unsigned char cardColour;
		unsigned char cardCmd;
		unsigned char totleLifes;//本卡可刷卡次数
		unsigned char activeTimes;//激活时长等级
		unsigned char rsv[14];
	}fields;
	unsigned char BYTES[20];
}_RC522_READ_BUFFER;
typedef union{
	struct{
		unsigned char remainLifes;
		unsigned int  gameStartMoment;
		unsigned char rsv[17];
	}fields;
	unsigned char BYTES[20];
}_RC522_WRITE_BUFFER;
typedef enum{
	CARD_TYPE_PRIMARY_PLAYER,
	CARD_TYPE_NORMAL_PLAYER,
	CARD_TYPE_SENIOR_PLAYER,
	CARD_TYPE_SUPER_PLAYER,
	CARD_TYPE_GAME_MASTER,
	CARD_TYPE_ADMIN,
	CARD_ROOM1,
}_CARD_TYPE;
typedef enum{
	CARD_COLOR_RED,
	CARD_COLOR_YELLOW,
	CARD_COLOR_BLUE,
	CARD_COLOR_GREEN,
	CARD_COLOR_WHITE,
	CARD_COLOR_BLACK,
}_CARD_COLOUR;
typedef enum{
	CARD_CMD_RESET_ME,
	CARD_CMD_RESET_ALL,
	CARD_CMD_OPEN_DOOR,
}_CARD_CMD;
#endif /* CONFIG_H_ */
