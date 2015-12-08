// Do not remove the include below
#include "main.h"

void initIO() {

#ifdef ON_BOARD_MOS
	pinMode(output1Pin, OUTPUT);
	digitalWrite(output1Pin, MOS_INIT_STATE);
	pinMode(output2Pin, OUTPUT);
	digitalWrite(output2Pin, MOS_INIT_STATE);
	pinMode(output3Pin, OUTPUT);
	digitalWrite(output3Pin, MOS_INIT_STATE);
	pinMode(output4Pin, OUTPUT);
	digitalWrite(output4Pin, MOS_INIT_STATE);
#if ON_BOARD_MOS>4
	pinMode(output5Pin, OUTPUT);
	digitalWrite(output5Pin, MOS_INIT_STATE);
	pinMode(output6Pin, OUTPUT);
	digitalWrite(output6Pin, MOS_INIT_STATE);
#if ON_BOARD_MOS>6
	pinMode(output7Pin,OUTPUT);
	digitalWrite(output7Pin,MOS_INIT_STATE);
	pinMode(output8Pin,OUTPUT);
	digitalWrite(output8Pin,MOS_INIT_STATE);
#endif
#endif
#endif

#ifdef ON_BOARD_INPUT
	pinMode(input1Pin, INPUT_PULLUP);
	pinMode(input2Pin, INPUT_PULLUP);
#if ON_BOARD_INPUT>2
	pinMode(input3Pin, INPUT_PULLUP);
#if ON_BOARD_INPUT>3
	pinMode(input4Pin,INPUT_PULLUP);
	pinMode(input5Pin,INPUT_PULLUP);
#if ON_BOARD_INPUT>5
	pinMode(input6Pin,INPUT_PULLUP);
	pinMode(input7Pin,INPUT_PULLUP);
	pinMode(input8Pin,INPUT_PULLUP);
	pinMode(input9Pin,INPUT_PULLUP);
	pinMode(input10Pin,INPUT_PULLUP);
#endif
#endif
#endif
#endif

#ifdef ON_BOARD_BUZZ
	pinMode(buzzPin, OUTPUT);
	digitalWrite(buzzPin, BUZZ_OFF);
#endif

#ifdef ON_BOARD_433MHz_SW
#if ME_ADDRESS==1
	pinMode(set433Pin, OUTPUT);
	digitalWrite(set433Pin, HIGH);
#endif
#endif
}
void initVariables() {
	gFlag.all = 0;
#ifdef ON_BOARD_RC522
	for (byte i = 0; i < 6; i++)
		key.keyByte[i] = 0xFF;
	for (byte i = 0; i < sizeof(RC522ReadBuffer); i++) {
		RC522ReadBuffer.BYTES[i] = 0xff;
		RC522WriteBuffer.BYTES[i] = 0xff;
		RC522ID[i] = 0xff;
		RC522ActiveMoment = 0;
		RC522LastID = 0xff;
	}
#endif
	for (unsigned char i = 0; i < TOTAL_TIMER_NUM; i++) {
		gTimer[i] = 0;
	}
	gState = STATE_WAITING;
	failCount = 0;
}
void resetMyself(void) {
	if (gState == STATE_RECOVERING) {
		static unsigned char count=0;
		count++;
		if(count>=failCount){
			count=0;
			myTimer.KillTimer(&((unsigned char)gTimer[DEAD_TIMER]));
		}
		else
			return;
	}
	initIO();
	initVariables();
}
#ifdef ON_BOARD_RC522
/******************************************************
 * Clear Buffer
 */
void clearRC522Buffer(unsigned char block) {

	if (block == READ_BLOCK) {
		for (byte i = 0; i < sizeof(RC522ReadBuffer); i++) {
			RC522ReadBuffer.BYTES[i] = 0xff;
		}
	} else if (block == WRITE_BLOCK) {
		for (byte i = 0; i < sizeof(RC522WriteBuffer); i++) {
			RC522WriteBuffer.BYTES[i] = 0xff;
		}
	}
}
/******************************************************
 * Handle the command from the card
 */
bool RC522CmdHandle(unsigned char cmd) {
	switch (cmd) {
	case CARD_CMD_RESET_ME:
		resetMyself();
		return 1;
		break;
	case CARD_CMD_RESET_ALL:
		for (unsigned char i = 1; i < TOTOL_DEVICE_NUMBER; i++) {
//			myComm.sendCMD(i,COMM_RESET_ME,0);
			delay(50);
		}
		resetMyself();
		return 1;
		break;
	case CARD_CMD_OPEN_DOOR:
		return 1;
		digitalWrite(output1Pin, DOOR_OPEN);
		gTimer[OPEN_DOOR_TIMER] = myTimer.SetTimer(OPEN_DOOR_LEN, resetMyself);
		gState = STATE_ACTING;
		break;
	default:
		break;
	}
	return 0;
}
/******************************************************
 * Read the read-only area to RC522Buffer and return 0 if success
 */
unsigned char readCard() {
	byte status;
	byte address = sizeof(RC522ReadBuffer);

	if (!mfrc522.PICC_IsNewCardPresent())
		return 1;

	if (!mfrc522.PICC_ReadCardSerial())
		return 2;

	status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A,
			READ_BLOCK, &key, &(mfrc522.uid));
	if (status != MFRC522::STATUS_OK)
		return 3;

	status = mfrc522.MIFARE_Read(READ_BLOCK, &RC522ReadBuffer.BYTES[0],
			&address);
	if (status != MFRC522::STATUS_OK)
		return 4;
	status = mfrc522.MIFARE_Read(WRITE_BLOCK, &RC522WriteBuffer.BYTES[0],
			&address);
	if (status != MFRC522::STATUS_OK)
		return 5;

	if (RC522ReadBuffer.fields.cardType == CARD_TYPE_GAME_MASTER
			|| RC522ReadBuffer.fields.cardType == CARD_TYPE_ADMIN) {
		RC522CmdHandle(RC522ReadBuffer.fields.cardCmd);
	}

	mfrc522.PICC_HaltA(); // Halt PICC
	mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD

	return 0;

}
/******************************************************
 * Write the content of writeBuffer into card data area and return 0 if success
 * You need modify writeBuffer first,and writeBuffer will be set as 0xff if success
 */
unsigned char writeCard() {
	byte status;

	if (!mfrc522.PICC_WakeupACard())
		return 1;

	if (!mfrc522.PICC_ReadCardSerial())
		return 2;

	status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A,
			WRITE_BLOCK, &key, &(mfrc522.uid));
	if (status != MFRC522::STATUS_OK)
		return 3;

	status = mfrc522.MIFARE_Write(WRITE_BLOCK, &RC522WriteBuffer.BYTES[0], 16);
	if (status != MFRC522::STATUS_OK)
		return 4;
	mfrc522.PICC_HaltA(); // Halt PICC
	mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD

	return 0;
}
#endif
/*****************Timer Callback**********************************************************************************/
void openDoor_Handle() {
	digitalWrite(output1Pin, DOOR_CLOSE);
	gState = STATE_WAITING;
}
void wrongCardRecovery_Handle() {
	gState = STATE_WAITING;
}
/******************************************************************************************/
void TimingISR(word interval) {
	static unsigned long ISRmoment = 0;

	if ((millis() - ISRmoment) > interval) {
		ISRmoment = millis();
	} else
		return;

	wdt_reset();
	myTimer.processTimer();
#ifdef ON_BOARD_RC522
	unsigned char temp = readCard();
	if (temp == 0) {
		gFlag.bit.IsNewCard = 1;
	}
#endif
}
//The setup function is called once at startup of the sketch
void setup() {
// Add your initialization code here
	initIO();
	initVariables();
	Serial.begin(9600);
//	myComm.begin(9600);
#ifdef ON_BOARD_RC522
	SPI.begin();
	mfrc522.PCD_Init();        // Init MFRC522 card
#endif

#ifdef ON_BOARD_MP3
	myMp3.write(PAUSE_PLAY£¬NONE_SONG);
	myMp3.write(SET_VOLUMN,30);
#endif
	myOpenID = (digitalRead(input1Pin) + digitalRead(input2Pin) * 2
			+ digitalRead(input3Pin) * 4) + 1;
	for (unsigned char i = 0; i < myOpenID; i++) {
		digitalWrite(buzzPin, BUZZ_ON);
		delay(500);
		digitalWrite(buzzPin, BUZZ_OFF);
		delay(500);
	}

	digitalWrite(buzzPin, BUZZ_OFF);
	wdt_enable(WDTO_8S);
}
/***************System State************************************************************/
void stateWaiting() {
	if (gFlag.bit.IsNewCard == 0)
		return;
	if (RC522ReadBuffer.fields.cardType == CARD_ROOM1) {
		delay(500);
		if (RC522ReadBuffer.fields.cardNumber == myOpenID) {
			digitalWrite(buzzPin, BUZZ_ON);
			delay(1000);
			digitalWrite(buzzPin, BUZZ_OFF);
			delay(500);
			digitalWrite(output1Pin, DOOR_OPEN);
			gTimer[OPEN_DOOR_TIMER] = myTimer.SetTimer(OPEN_DOOR_LEN,
					resetMyself);
			gState = STATE_ACTING;
		} else {
			for (unsigned char i = 0; i < 3; i++) {
				digitalWrite(buzzPin, BUZZ_ON);
				delay(150);
				digitalWrite(buzzPin, BUZZ_OFF);
				delay(150);
			}
			failCount++;
			gTimer[DEAD_TIMER] = myTimer.SetTimer(DEAD_TIME, resetMyself, 1);
			gState = STATE_RECOVERING;
		}
	}
}
void stateActing() {

}
void stateRecovering() {
}
void stateRetry() {

}
/******************************************************************************************/
// The loop function is called in an endless loop
void loop() {
	//Add your repeated code here
	switch (gState) {
	case STATE_WAITING:
		stateWaiting();
		break;
	case STATE_ACTING:
		stateActing();
		break;
	case STATE_RECOVERING:
		stateRecovering();
		break;
	case STATE_RETRY:
		stateRetry();
		break;
	default:
		gState = STATE_WAITING;
		break;
	}
	//************Read Card*******************/
	TimingISR(200);	//100ms
}
