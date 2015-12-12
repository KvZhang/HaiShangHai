// Do not remove the include below
#include "LaputaSetting.h"

MFRC522 mfrc522(10, 9);        // Create MFRC522 instance.
MFRC522::MIFARE_Key key;
SoftwareSerial settingSerial(2, 3);
//The setup function is called once at startup of the sketch
void displayCard() {
	for (unsigned char i = 0; i < 16; i++) {
		Serial.print(RC522ReadBuffer.BYTES[i], HEX);
		Serial.print(' ');
	}
	Serial.println(' ');
	switch (RC522ReadBuffer.fields.cardType) {
	case CARD_TYPE_NORMAL_PLAYER:
	case CARD_TYPE_PRIMARY_PLAYER:
	case CARD_TYPE_SENIOR_PLAYER:
	case CARD_TYPE_SUPER_PLAYER:
		Serial.print("Player");
		Serial.print(" ");
		Serial.print(RC522ReadBuffer.fields.cardNumber);
		Serial.print('|');
		Serial.print("Color->");
		if (RC522ReadBuffer.fields.cardColour == CARD_COLOR_RED) {
			Serial.print("RED");
		} else if (RC522ReadBuffer.fields.cardColour == CARD_COLOR_GREEN) {
			Serial.print("GREEN");
		} else if (RC522ReadBuffer.fields.cardColour == CARD_COLOR_BLUE) {
			Serial.print("BLUE");
		}
		Serial.print('|');
		Serial.print("Lives->");
		Serial.print(RC522ReadBuffer.fields.totleLifes);
		Serial.print('|');
		Serial.print("ActiveLvl->");
		Serial.print(RC522ReadBuffer.fields.activeTimes);
		Serial.println(' ');
		break;
	case CARD_TYPE_ADMIN:
	case CARD_TYPE_GAME_MASTER:
		Serial.print("GameMaster->");
		if (RC522ReadBuffer.fields.cardCmd == CARD_CMD_RESET_ME) {
			Serial.print("RESET THIS");
		} else if (RC522ReadBuffer.fields.cardCmd == CARD_CMD_RESET_ALL) {
			Serial.print("RESET ALL");
		} else if (RC522ReadBuffer.fields.cardCmd == CARD_CMD_OPEN_DOOR) {
			Serial.print("ACTIVE");
		}
		Serial.println(' ');
		break;
	default:
		Serial.println("Unknown card");
		break;
	}
}
void processCMD(unsigned char cmd) {
	unsigned char num = 0;
	unsigned char i = 0;
	unsigned char temp=0;
	switch (cmd) {
	case 'T':
	case 't':
		temp = Serial.read();
		if (temp == 'p' || temp == 'P') {
			RC522ReadBuffer.fields.cardType = CARD_TYPE_NORMAL_PLAYER;
		} else if (temp == 'g' || temp == 'G') {
			RC522ReadBuffer.fields.cardType = CARD_TYPE_GAME_MASTER;
		} else {
			RC522ReadBuffer.fields.cardType = CARD_TYPE_GAME_MASTER;
		}
		break;
	case 'n':
	case 'N':
		num=0;
		i=0;
		while (Serial.available()) {
			temp = Serial.read();
			if (temp == ' ') {
				RC522ReadBuffer.fields.cardNumber = 1;
				break;
			} else {
				num = num * 10 * i + temp - '0';
				i++;
			}
		}
		if (num > 10) {
			RC522ReadBuffer.fields.cardNumber = 1;
		} else {
			RC522ReadBuffer.fields.cardNumber = num;
		}
		break;
	case 'c':
	case 'C':
		temp = Serial.read();
		if (temp == 'R' || temp == 'r') {
			RC522ReadBuffer.fields.cardColour = CARD_COLOR_RED;
		} else if (temp == 'B' || temp == 'b') {
			RC522ReadBuffer.fields.cardColour = CARD_COLOR_BLUE;
		} else if (temp == 'G' || temp == 'g') {
			RC522ReadBuffer.fields.cardColour = CARD_COLOR_GREEN;
		} else {
			RC522ReadBuffer.fields.cardColour = CARD_COLOR_RED;
		}
		break;
	case 'p':
	case 'P':
		temp = Serial.read();
		if (temp == 'R' || temp == 'r') {
			RC522ReadBuffer.fields.cardCmd = CARD_CMD_RESET_ME;
		} else if (temp == 'A' || temp == 'a') {
			RC522ReadBuffer.fields.cardCmd = CARD_CMD_OPEN_DOOR;
		} else {
			RC522ReadBuffer.fields.cardCmd = CARD_CMD_RESET_ME;
		}
		break;
	case 'L':
	case 'l':
		num = 0;
		i = 0;
		while (Serial.available()) {
			temp = Serial.read();
			if (temp == ' ') {
				RC522ReadBuffer.fields.totleLifes = 10;
				break;
			} else {
				num = num * 10 * i + temp - '0';
				i++;
			}
		}
		if (num > 100) {
			RC522ReadBuffer.fields.totleLifes = 10;
		} else {
			RC522ReadBuffer.fields.totleLifes = num;
		}
		break;
	case 'a':
	case 'A':
		num = 0;
		i = 0;
		while (Serial.available()) {
			temp = Serial.read();
			if (temp == ' ') {
				RC522ReadBuffer.fields.activeTimes = 1;
				break;
			} else {
				num = num * 10 * i + temp - '0';
				i++;
			}
		}
		if (num > 10) {
			RC522ReadBuffer.fields.activeTimes = 1;
		} else {
			RC522ReadBuffer.fields.activeTimes = num;
		}
		break;
	}

}

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
void setup() {
// Add your initialization code here
	pinMode(setPin, OUTPUT);
	digitalWrite(setPin, HIGH);
	Serial.begin(9600);
	SPI.begin();                // Init SPI bus
	mfrc522.PCD_Init();        // Init MFRC522 card
	for (byte i = 0; i < 6; i++)
		key.keyByte[i] = 0xFF;
}
void setDefault() {
	settingSerial.begin(settingBRD);
	unsigned char mySerialRxBuffer[10] = { 0 };
	digitalWrite(setPin, LOW);
	delay(100);
	settingSerial.println("AT+C20");
	delay(100);

	Serial.print("Now setting channels 20");
	Serial.println("......");
	Serial.print('.');
	unsigned char i = 0;
	while (settingSerial.available() == 0) {
		i++;
		delay(100);
		Serial.print('.');
		if (i > 20) {
			Serial.println(" ");
			Serial.println("Setting failed, over time...");
			Serial.println(" ");
			return;
		}
	}
	i = 0;
	while (settingSerial.available()) {
		mySerialRxBuffer[i] = settingSerial.read();
		i++;
	}
	if (mySerialRxBuffer[0] == 'O' && mySerialRxBuffer[1] == 'K') {
		Serial.println("Setting success!");
		Serial.println(" ");
	} else {
		Serial.println("Setting failed, unknown error...");
		Serial.println(" ");
		return;
	}
	delay(100);
	settingSerial.println("AT+B19200");
	delay(100);

	Serial.print("Now setting baud rate 19200");
	Serial.println("......");
	Serial.print('.');
	i = 0;
	while (settingSerial.available() == 0) {
		i++;
		delay(100);
		Serial.print('.');
		if (i > 20) {
			Serial.println(" ");
			Serial.println("Setting failed, over time...");
			Serial.println(" ");
			return;
		}
	}
	i = 0;
	while (settingSerial.available()) {
		mySerialRxBuffer[i] = settingSerial.read();
		i++;
	}
	if (mySerialRxBuffer[0] == 'O' && mySerialRxBuffer[1] == 'K') {
		Serial.println("Setting success!");
		Serial.println(" ");
	} else {
		Serial.println("Setting failed, unknown error...");
		Serial.println(" ");
	}
	digitalWrite(setPin, HIGH);
}
void setChannel() {
	settingSerial.begin(commBRD);
	unsigned char mySerialRxBuffer[10] = { 0 };
	unsigned char channel = 0;
	digitalWrite(setPin, LOW);

	Serial.println("Put the channel you want to set(001~127,char):");

	while (Serial.available() == 0)
		;
	delay(100);
	while (Serial.available() > 0) {
		unsigned char temp = Serial.read();
		if (temp >= '0' && temp <= '9') {
			channel = channel * 10 + (temp - '0');
		}
	}
	if (channel >= 1 && channel <= 127) {

		delay(100);
		settingSerial.print("AT+C");
//		if (channel < 10) {
//			settingSerial.print("00");
//		} else if (channel < 100) {
//			settingSerial.print('0');
//		}
		settingSerial.println(channel);

		Serial.print("Now setting channel as ");
//		if (channel < 10) {
//			Serial.print("00");
//		} else if (channel < 100) {
//			Serial.print('0');
//		}
		Serial.print(channel);
		Serial.println("......");
		Serial.print('.');
		unsigned char i = 0;
		while (settingSerial.available() == 0) {
			i++;
			delay(100);
			Serial.print('.');
			if (i > 20) {
				Serial.println(" ");
				Serial.println("Setting failed, over time...");
				Serial.println(" ");
				return;
			}
		}
		i = 0;
		while (settingSerial.available()) {
			mySerialRxBuffer[i] = settingSerial.read();
			i++;
		}
		if (mySerialRxBuffer[0] == 'O' && mySerialRxBuffer[1] == 'K') {
			Serial.println("Setting success!");
			Serial.println(" ");
		} else {
			Serial.println("Setting failed, unknown error...");
			Serial.println(" ");
		}

	} else {
		Serial.print(channel);
		Serial.println(" is not a valid channel number!");
		Serial.println(" ");
	}
	digitalWrite(setPin, HIGH);
}
void RWRC522() {
	byte block = 0;
	byte offset = 0;
	byte len = 20;
	byte buffer[20] = { 0 };
	Serial.println("Now put your card on the board!");
	while (!mfrc522.PICC_IsNewCardPresent())
		;
	while (!mfrc522.PICC_ReadCardSerial())
		;

	Serial.print("Card UID:");    //Dump UID
	for (byte i = 0; i < mfrc522.uid.size; i++) {
		Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
		Serial.print(mfrc522.uid.uidByte[i], HEX);
	}
	Serial.print(" PICC type: ");   // Dump PICC type
	byte piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
	Serial.println(piccType);
	/******************************Set Block Number************************/
	Serial.println("Put the block number: 0~63 ");
	while (Serial.available() == 0)
		;
	delay(100);
	while (Serial.available() > 0) {
		block = block * 10 + Serial.read() - '0';
	}
	if (block <= 63) {
		if ((block + 1) % 4) {
			Serial.print("Selected block ");
			Serial.println(block);
		} else {
			Serial.println("Read-only block...");
			mfrc522.PICC_HaltA(); // Halt PICC
			mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
			return;
		}
	} else {
		Serial.println("Wrong block number!");
		mfrc522.PICC_HaltA(); // Halt PICC
		mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
		return;
	}
	//Serial.println("Authenticating using key A...");
	unsigned char status;
	status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block,
			&key, &(mfrc522.uid));
	if (status != MFRC522::STATUS_OK) {
		Serial.print("PCD_Authenticate() failed: ");
		Serial.println(status);
		return;
	}
	/*****************************Set offset************************************/
	Serial.println("Put the offset: 0~15(0 means block r/w) ");
	while (Serial.available() == 0)
		;
	delay(100);
	while (Serial.available() > 0) {
		offset = offset * 10 + Serial.read() - '0';
	}
	if (block <= 63) {
		Serial.print("Set offset as : ");
		Serial.println(offset);
	} else {
		Serial.println("Wrong block number!");
		mfrc522.PICC_HaltA(); // Halt PICC
		mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
		return;
	}
	/******************************Set R/W*****************************************/

	Serial.println("Which action do you want: 1, Read;2, Write");
	while (Serial.available() == 0)
		;
	delay(100);
	while (Serial.available() > 0) {
		unsigned char temp = Serial.read();
		if (temp == '1' || temp == 1) {
			unsigned char status;
			status = mfrc522.MIFARE_Read(block, buffer, &len);
			if (status != 1) {
				Serial.println("Read error! code:");
				Serial.println(status);
				mfrc522.PICC_HaltA(); // Halt PICC
				mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
				return;
			}
			if (offset) {
				Serial.print("The read result is:");
				Serial.print(buffer[offset], HEX);
				Serial.print(' ');
				Serial.println("  ");
			} else {
				Serial.print("The read result is:");
				for (unsigned char i = 0; i < 16; i++) {
					Serial.print(buffer[offset], HEX);
					Serial.print(' ');
				}
				Serial.println("  ");
				Serial.println("  ");
			}
		} else if (temp == '2' || temp == 2) {
			Serial.println(
					"Put the content you want to write(only first 16 Bytes is useful);");
			while (!Serial.available())
				;
			delay(100);
			unsigned char i = 0;
			while (Serial.available()) {
				buffer[i + offset] = Serial.read();
				i++;
				if (i + offset >= 16) {
					Serial.flush();
				}
			}
			if (mfrc522.MIFARE_Write(block, buffer, 16)) {
				Serial.print("Write Success with : ");
				for (unsigned char j = 0; j < i; j++) {
					Serial.print(buffer[j + offset], HEX);
					Serial.print(' ');
				}
				Serial.println("  ");
				Serial.println("  ");
			} else {
				Serial.println("Write error!");
				mfrc522.PICC_HaltA(); // Halt PICC
				mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
				return;
			}
		}
	}
	mfrc522.PICC_HaltA(); // Halt PICC
	mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
}
void bootLoader() {
	myComm.begin(commBRD);
	Serial.println(
			"Now put the object address you want to upload use below format:");
	Serial.println("ObjTheme ObjAddr   2 numbers separated by blank!!");
	while (Serial.available() == 0)
		;
	delay(10);
	myCommBuffer.objTheme = 0;
	myCommBuffer.objAddr = 0;
	while (Serial.available() > 0) {
		unsigned char temp;
		temp = Serial.read();
		if (temp == ' ')
			break;
		myCommBuffer.objTheme = (temp - '0') + myCommBuffer.objTheme * 10;
	}
	while (Serial.available() > 0) {
		unsigned char temp;
		temp = Serial.read();
		if (temp > '9' || temp < '0') {
			Serial.flush();
			break;
		}
		myCommBuffer.objAddr = (temp - '0') + myCommBuffer.objAddr * 10;
	}
	if (myCommBuffer.objTheme > 0 && myCommBuffer.objTheme < 100) {
		if (myCommBuffer.objAddr > 0 && myCommBuffer.objAddr < 100) {
			myComm.sendCMD(0, 0, STOP_REPORT, 1);  //重要的事情说三遍
			delay(200);
			myComm.sendCMD(0, 0, STOP_REPORT, 1);
			delay(200);
			myComm.sendCMD(0, 0, STOP_REPORT, 1);
			delay(200);
			myComm.sendCMD(myCommBuffer.objTheme, myCommBuffer.objAddr,
					GOTO_BOOTLOADER, 0);
			delay(200);
		} else {
			Serial.println("Wrong address number!");
		}
	} else
		Serial.println("Wrong theme number!");

}
// The loop function is called in an endless loop
void loop() {
//Add your repeated code here
	Serial.println(F("***********************************"));
	Serial.println(F("  Choose your wanted operation: "));
	Serial.println(F(">>1: Read/write IC card "));
	Serial.println(F(">>2: Set channel of 433MHz "));
	Serial.println(F(">>3: Set 433 19200/ch20 "));
	Serial.println(F(">>4: Wireless unloading "));
	Serial.println(F(">>5: Wireless uploading finished "));
	Serial.println(F(">>6: Send commands to device "));
	Serial.println(F(">>7: Read card context for Haishanghai "));
	Serial.println(F(">>8: Card Setting for Haishanghai "));

	while (Serial.available() == 0)
		;
	delay(10);
	while (Serial.available() > 0) {
		unsigned char temp = Serial.read();
		if (temp == '1' || temp == 1) {
			Serial.flush();
			RWRC522();
		} else if (temp == '2' || temp == 2) {
			Serial.flush();
			setChannel();
		} else if (temp == '3' || temp == 3) {
			Serial.flush();
			setDefault();
		} else if (temp == '4' || temp == 4) {
			Serial.flush();
			bootLoader();
		} else if (temp == '5' || temp == 5) {
			myComm.begin(commBRD);
			myComm.sendCMD(0, 0, STOP_REPORT, 0);
		} else if (temp == '6' || temp == 6) {
			Serial.println(
					"As format:themeAddr ObjAddr cmd dataH dataL(5 bytes in Hex)");
			delay(1000);
			while (Serial.available() < 5)
				;
			myComm.begin(commBRD);
			myComm.sendCMD(Serial.read(), Serial.read(), Serial.read(),
					Serial.read() * 256 + Serial.read());
		} else if (temp == '7' || temp == 7) {
			Serial.println("Pls put your card");
			while (readCard()){
				delay(100);
			}
			Serial.println("Content is:");
			delay(100);
			displayCard();
		} else if (temp == '8' || temp == 8) {
			Serial.println("Pls put your card");
			clearRC522Buffer(READ_BLOCK);
			while (!mfrc522.PICC_IsNewCardPresent()){
				delay(100);
			}
			while (!mfrc522.PICC_ReadCardSerial()){
				delay(100);
			}

			Serial.print("Card UID:");    //Dump UID
			for (byte i = 0; i < mfrc522.uid.size; i++) {
				Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
				Serial.print(mfrc522.uid.uidByte[i], HEX);
			}
			Serial.println("Put the setting parameters like below:");
			Serial.println("-tP/G(type, default player,needed)\n"
					"-nxx(xx:card number 1~8,default 1)\n"
					"-cR/G/B(color,default red)\n"
					"-pR/A(reset or active, default reset)\n"
					"-lxx(xx:lifes, default 10)\n"
					"-axx(xx:active level,default 1)\n"
					"set a player card:-tP -n2 -cR (-l9 -a3)\n"
					"set a GM card:-tG -pR");
			while (Serial.available() == 0)
				;
			delay(100);
			while (Serial.available()) {
				if (Serial.read() == '-')
					processCMD(Serial.read());
			}
			if (RC522ReadBuffer.fields.cardType == CARD_TYPE_GAME_MASTER) {
				if (RC522ReadBuffer.fields.cardCmd == CARD_CMD_RESET_ME
						|| RC522ReadBuffer.fields.cardCmd
								== CARD_CMD_OPEN_DOOR) {

				} else {
					RC522ReadBuffer.fields.cardCmd = CARD_CMD_RESET_ME;
				}
				RC522ReadBuffer.fields.activeTimes=0xff;
				RC522ReadBuffer.fields.cardColour=0xff;
				RC522ReadBuffer.fields.cardNumber=0xff;
				RC522ReadBuffer.fields.totleLifes=0xff;
			} else if (RC522ReadBuffer.fields.cardType
					== CARD_TYPE_NORMAL_PLAYER) {
				if (RC522ReadBuffer.fields.activeTimes == 0xff) {
					RC522ReadBuffer.fields.activeTimes = 1;
				}
				if (RC522ReadBuffer.fields.cardColour == 0xff) {
					RC522ReadBuffer.fields.cardColour = CARD_COLOR_RED;
				}
				if (RC522ReadBuffer.fields.cardNumber == 0xff) {
					RC522ReadBuffer.fields.cardNumber = 1;
				}
				if (RC522ReadBuffer.fields.totleLifes == 0xff) {
					RC522ReadBuffer.fields.totleLifes = 10;
				}
				RC522ReadBuffer.fields.cardCmd=0xff;
			}else{
				Serial.println("Must setting -t(Type)");
				break;
			}
			if(mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A,
					READ_BLOCK, &key, &(mfrc522.uid))!=1){
				Serial.println("Authenticate error!");
			}else{
				if(mfrc522.MIFARE_Write(READ_BLOCK, &RC522ReadBuffer.BYTES[0], 16)!=1){
					Serial.println("Write error!");
				}else{
					Serial.println("Successfully to set card as:");
					displayCard();
				}
			}

		}

		Serial.flush();
	}
	setup();
}
