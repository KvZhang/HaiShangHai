// Do not remove the include below
#include "main.h"

void outputLED(unsigned char i,bool status){//active channel i, i from 0 to 7,LED_ON/LED_OFF means
	if(i<6){
		digitalWrite(output1Pin+i,status);
	}else if(i==6){
		digitalWrite(output7Pin,status);
	}else if(i==7){
		digitalWrite(output8Pin,status);
	}
}
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
	pinMode(input3Pin,INPUT_PULLUP);
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
	gState=STATE_WAITING;
	gameStartMoment=0;
	totalPlayerNum=8;
}
void resetMyself(void) {
//	while (1)
//		; //waiting for watch dog reset
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
	switch(cmd){
	case CARD_CMD_RESET_ME:
		resetMyself();
		break;
	case CARD_CMD_RESET_ALL:
		if(ME_ADDRESS!=1){
			resetMyself();
			break;
		}
		for(unsigned char i=2;i<TOTOL_DEVICE_NUMBER;i++){
			myComm.sendCMD(i,COMM_RESET_ME,0);
			delay(50);
		}
		resetMyself();
		break;
	case CARD_CMD_OPEN_DOOR:
		myComm.sendCMD(DOOR_ADDR,COMM_OPEN_DOOR,0);
		return 1;
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

	if (RC522ReadBuffer.fields.cardType==CARD_TYPE_GAME_MASTER||RC522ReadBuffer.fields.cardType==CARD_TYPE_ADMIN){
		RC522CmdHandle(RC522ReadBuffer.fields.cardCmd);
		return 6;
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
//	byte address = sizeof(RC522WriteBuffer);

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
void activeTimeoutHandle(){
	if((millis()/1000-gameStartMoment)>1200){//1200s,20min time out
		myTimer.KillTimer(&(gTimer[ACTIVE_TIMEOUT]));
		initIO();
		initVariables();

	}
}
/******************************************************************************************/
void TimingISR(word interval) {
	static unsigned long ISRmoment = 0;

	if ((millis() - ISRmoment) > interval) {
		ISRmoment = millis();
	} else
		return;

	wdt_reset();
	if(readCard()==0)
		gFlag.bit.isNewcard=1;
	if (Serial.available() > 0) {
		unsigned char temp = Serial.read();
		switch (temp) {
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
			break;
		case NODEMCU_ACK:
			break;
		case NODEMCU_OPEN:
			break;
		case NODEMCU_QUESTION:
			break;
		case NODEMCU_REST:
			resetMyself();
			break;
		default:
			break;
		}
	}


	myTimer.processTimer();
}
//The setup function is called once at startup of the sketch
void setup() {
// Add your initialization code here
	initIO();
	initVariables();
	Serial.begin(9600);
	myComm.begin(19200);
#ifdef ON_BOARD_RC522
	SPI.begin();
	mfrc522.PCD_Init();        // Init MFRC522 card
#endif

#ifdef ON_BOARD_MP3
	myMp3.write(PAUSE_PLAY,NONE_SONG);
	myMp3.write(SET_VOLUMN,30);
#endif
	wdt_enable(WDTO_8S);
}
/***************System State************************************************************/
void stateWaiting() {
	if(myComm.flag.bit.isNeedProcess){
		if(myComm.cmdHandler(&myCommRevContent)){
			if(myCommRevContent.CMD==COMM_GAME_START){

				for(unsigned char i=0;i<sizeof(RC522ID);i++){
					if(RC522ID[i]==0xff){
						if(i<=6){//means still have 1~8 players
							totalPlayerNum=8-i;
							for(unsigned char j=2;j<8;j++){
								myComm.sendCMD(j,MASTER_ADDR,COMM_TOTLE_PERSON_NUM,totalPlayerNum);
								delay(150);
							}
						}
					}
				}
				gState=STATE_ACTING;
				gameStartMoment=millis()/1000;//current time in second
				myComm.sendCMD(MP3_ADDR,COMM_SINGLE_PLAY,SONG_INTRODUCE);
//				gFlag.bit.isNewCard=0;
				gTimer[ACTIVE_TIMEOUT]=myTimer.SetTimer(60000,activeTimeoutHandle,1);
			}
		}
	}
	if(gFlag.bit.isNewcard){
		for(unsigned char i=0;i<sizeof(RC522ID);i++){
			if(RC522ID[i]==0xff){
				RC522ID[i]=RC522ReadBuffer.fields.cardNumber;
				RC522LastID=RC522ReadBuffer.fields.cardNumber;
				digitalWrite(buzzPin,BUZZ_ON);
				delay(200);
				digitalWrite(buzzPin,BUZZ_OFF);
				outputLED(i,LED_ON);

				break;
			}
		}
		gFlag.bit.isNewcard=0;
		clearRC522Buffer(READ_BLOCK);
	}
}
void stateActing() {
	if(gFlag.bit.isNewcard){
		myTimer.KillTimer(&(gTimer[ACTIVE_TIMEOUT]));
		for(unsigned char i=0;i<sizeof(RC522ID);i++){
			if(RC522ID[i]==0xff){
				RC522ID[i]=RC522ReadBuffer.fields.cardNumber;
				RC522LastID=RC522ReadBuffer.fields.cardNumber;
				digitalWrite(buzzPin,BUZZ_ON);
				delay(200);
				digitalWrite(buzzPin,BUZZ_OFF);
				outputLED(i,LED_ON);
				RC522WriteBuffer.fields.gameStartMoment=gameStartMoment;
				RC522WriteBuffer.fields.remainLifes=RC522ReadBuffer.fields.totleLifes;
				writeCard();
				if(i>=MAX_PERSON_NUM-1){
					myComm.sendCMD(MP3_ADDR,COMM_SINGLE_PLAY,SONG_OPENDOOR);
					myComm.sendCMD(DOOR_ADDR,COMM_OPEN_DOOR,0);
					gState=STATE_RECOVERING;
					gTimer[RECOVERY_TIMER]=myTimer.SetTimer(60000,resetMyself);
				}
				break;
			}
		}
		gFlag.bit.isNewcard=0;
		clearRC522Buffer(READ_BLOCK);
	}
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
	TimingISR(100);	//100ms
	//************other routine jobs
	myComm.receiveCMD();
}
