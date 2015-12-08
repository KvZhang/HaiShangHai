// Do not remove the include below
#include "testColorLed.h"

const int redPin=9;
const int bluePin=10;
const int greenPin=11;

const int selectPin=8;
volatile unsigned char redVal=000;
volatile unsigned char blueVal=000;
volatile unsigned char greenVal=000;
volatile bool laststatus=1;
//The setup function is called once at startup of the sketch
void setup()
{
// Add your initialization code here
	pinMode(redPin,OUTPUT);
	pinMode(bluePin,OUTPUT);
	pinMode(greenPin,OUTPUT);
	pinMode(selectPin,OUTPUT);

	digitalWrite(selectPin,LOW);
	analogWrite(redPin,redVal);
	analogWrite(bluePin,blueVal);
	analogWrite(greenPin,greenVal);
	Serial.begin(9600);
}

// The loop function is called in an endless loop
void loop()
{
//Add your repeated code here
	delay(10);
	digitalWrite(selectPin,laststatus);
	unsigned char temp=0;
	if(Serial.available()>0){
		temp=Serial.read();
	}
	switch(temp){
	case 'q':
		redVal+=10;
		break;
	case 'a':
		redVal-=10;
		break;
	case 'w':
		blueVal+=10;
		break;
	case 's':
		blueVal-=10;
		break;
	case 'e':
		greenVal+=10;
		break;
	case 'd':
		greenVal-=10;
		break;
	default:
		break;
	}
	analogWrite(redPin,redVal);
	analogWrite(bluePin,blueVal);
	analogWrite(greenPin,greenVal);
}
