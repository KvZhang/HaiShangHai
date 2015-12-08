// Do not remove the include below
#include "testJumpToBootloader.h"

const buzzPin=8;
//The setup function is called once at startup of the sketch
void setup()
{
// Add your initialization code here
	pinMode(13,OUTPUT);
	digitalWrite(13,HIGH);
}

// The loop function is called in an endless loop
void loop()
{
//Add your repeated code here
	if(millis()>10000){
		for(unsigned char i=0;i<3;i++){
			digitalWrite(13,LOW);
			delay(200);
			digitalWrite(13,HIGH);
			delay(200);
		}
		digitalWrite(13,LOW);
#if (BOARD==AVR_NANO)
		asm("JMP 14336 ");//0x3800
#elif (BOARD==AVR_UNO)
		asm("JMP 15872 ");//0x3E00
#else
		asm("JMP 15872 ");//0x3E00
#endif
	}
}
