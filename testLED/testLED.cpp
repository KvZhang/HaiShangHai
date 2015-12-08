// Do not remove the include below
#include "testLED.h"

MFRC522 mfrc522(10, 9);        // Create MFRC522 instance.
MFRC522::MIFARE_Key key;
//The setup function is called once at startup of the sketch
void setup()
{
// Add your initialization code here
}

// The loop function is called in an endless loop
void loop()
{
//Add your repeated code here
}
unsigned char readCardInfo(void) {
//	mfrc522.PCD_Init();
// Look for new cards
	if (!mfrc522.PICC_IsNewCardPresent())
		return 2;

// Select one of the cards
	if (!mfrc522.PICC_ReadCardSerial())
		return 1;

	byte buffer[20];
	byte block;
	byte status, len;
	byte temp;

	block = IDAddress;
	byte address = sizeof(buffer);

	status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block,
			&key, &(mfrc522.uid));
	if (status != MFRC522::STATUS_OK)
		return 3;

	status = mfrc522.MIFARE_Read(block, buffer, &address);
	if (status != MFRC522::STATUS_OK)
		return 4;
	temp = buffer[0];
	delay(10);
//	if (temp == currentChar) {
//		buffer[0] = nextChar;
//		status = mfrc522.MIFARE_Write(block, buffer, 16);
//	}

	mfrc522.PICC_HaltA(); // Halt PICC
	mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD

	return temp+10;
}
