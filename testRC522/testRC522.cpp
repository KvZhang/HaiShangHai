// Do not remove the include below
#include "testRC522.h"


#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);        // Create MFRC522 instance.

void setup() {
  Serial.begin(9600);        // Initialize serial communications with the PC
  SPI.begin();                // Init SPI bus
  mfrc522.PCD_Init();        // Init MFRC522 card
  //Serial.println("Scan a MIFARE Classic PICC to demonstrate Value Blocks.");
}

void loop() {
  static byte flag=0;
  delay(200);
  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

  byte status;
  byte RC522Buffer[20];
  byte address = sizeof(RC522Buffer);

  if (!mfrc522.PICC_WakeupACard()) {
    Serial.println(5);
    return;
  }

 /* if (!mfrc522.PICC_IsNewCardPresent()) {
    Serial.println(1);
    return;
  }*/

  if (!mfrc522.PICC_ReadCardSerial()) {
    Serial.println(2);
    return;
  }

  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 5,
                                    &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.println(3);
    return;
  }

  status = mfrc522.MIFARE_Read(5, RC522Buffer, &address);
  if (status != MFRC522::STATUS_OK) {

    Serial.println(4);
    return;
  }

  Serial.println(RC522Buffer[0]);
  flag=1;
  for(unsigned char i=0;i<sizeof(RC522Buffer);i++){
	  RC522Buffer[i]=0xff;
  }

  mfrc522.PICC_HaltA(); // Halt PICC
  mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD


}
