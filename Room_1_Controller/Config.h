/*
 * Config.h
 *
 *  Created on: 2015年11月5日
 *      Author: zhangyu
 */

#ifndef CONFIG_H_
#define CONFIG_H_

/*******Peripheral******************************************************************************/
#define ON_BOARD_MOS		8	//1~4,5~6,7~8 three options

//#define ON_BOARD_INPUT		2	//1~2,3,4~5,6~10 four options
#define ON_BOARD_RC522

//#define ON_BOARD_MP3
#define ON_BOARD_BUZZ

#define ON_BOARD_433MHz_SW


//#define ON_BOARD_433MHz_HW

//#define ON_BOARD_IIC
/*******Comm address******************************************************************************/
#define ROOM_ADDRESS 	(0x01)
#define ME_ADDRESS		(0x01)
typedef enum{
	MASTER_ADDR=1,
	DOOR_ADDR,
	MP3_ADDR,
	SLAVE_ONE,
	SLAVE_TWO,
	TOTOL_DEVICE_NUMBER,
}_MY_COMM_ADDRESS;
typedef enum {
	COMM_RESET_ME = 0x20,
	COMM_RESET_ALL,
	COMM_OPEN_DOOR,
	COMM_DOOR_OPENED,
	COMM_TOTLE_PERSON_NUM,
	COMM_SINGLE_PLAY,
	COMM_REPEAT_PLAY,
	COMM_STOP_PLAY,
	COMM_SET_VOLUME,
	COMM_GAME_START,
} _COMM_CMD_TYPE;
typedef enum{
	SONG_INTRODUCE=1,

	SONG_OPENDOOR,
	SONG_GOOD,
}_SONG_DEFINE;
/********Other define**********************************************************************************/
#define BUZZ_ON 		LOW
#define BUZZ_OFF		HIGH
#define MOS_INIT_STATE		0
#define MOS_ACTIVE_STATE	1
#define MAX_PERSON_NUM		8
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
//Timer type
typedef enum{
	ACTIVE_TIMEOUT,
	RECOVERY_TIMER,
	TOTAL_TIMER_NUM,
} _SYS_TIMER;
//RC522 Block define
typedef enum{
	READ_BLOCK=5,
	WRITE_BLOCK=6,
} _RC522_BLOCK;
//RC522 read buffer
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
//RC522 write buffer
typedef union{
	struct{
		unsigned char remainLifes;
		unsigned int  gameStartMoment;
		unsigned char rsv[17];
	}fields;
	unsigned char BYTES[20];
}_RC522_WRITE_BUFFER;
//Card type
typedef enum{
	CARD_TYPE_PRIMARY_PLAYER,
	CARD_TYPE_NORMAL_PLAYER,
	CARD_TYPE_SENIOR_PLAYER,
	CARD_TYPE_SUPER_PLAYER,
	CARD_TYPE_GAME_MASTER,
	CARD_TYPE_ADMIN,
}_CARD_TYPE;
//Card color
typedef enum{
	CARD_COLOR_RED,
	CARD_COLOR_YELLOW,
	CARD_COLOR_BLUE,
	CARD_COLOR_GREEN,
	CARD_COLOR_WHITE,
	CARD_COLOR_BLACK,
}_CARD_COLOUR;
//Card command
typedef enum{
	CARD_CMD_RESET_ME,
	CARD_CMD_RESET_ALL,
	CARD_CMD_OPEN_DOOR,
}_CARD_CMD;
#endif /* CONFIG_H_ */
