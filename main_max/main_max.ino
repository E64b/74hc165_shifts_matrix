/* =================Unlimited Shifts================= */
/* ==U can add 1 or more Shifts and read val in serial if val changed== */
#include "main.h"

void setup()
{
	Serial.begin(115200);
	while (!Serial){    
  }
	pinMode(ploadPin, OUTPUT);
	pinMode(clockEnablePin, OUTPUT);
	pinMode(dataPin, INPUT);
	pinMode(clockPin, OUTPUT);
}

/* Тот самый пример

	if (!update) {
		return;
	}

	update = false;
	for (int i = 0; i < shifts; i++)
	{
		Serial.printl(Shift[i], HEX); Send current val HEX
		Serial.print(' ');
	}
	Serial.println();
*/


void loop(){
	read();	//Read data
	checkData(); //Checking data for changes
	displayValues(); //Send to serial	
	delay(10);
}
