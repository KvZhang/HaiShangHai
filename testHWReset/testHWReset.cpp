// Do not remove the include below
#include "testHWReset.h"


//The setup function is called once at startup of the sketch
void setup()
{
// Add your initialization code here
	wdt_enable(WDTO_1S); //���Ź�ʹ�� �����ó�ʱʱ��
	wdt_reset();
	Serial.begin(9600);
	pinMode(13,OUTPUT);
//	digitalWrite(13,LOW);
}

// The loop function is called in an endless loop
void loop()
{
//Add your repeated code here
	i++;
	delay(10);
	if(i>50) digitalWrite(13,HIGH);
	Serial.println(i);
}
