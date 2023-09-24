/* =================Unlimited Shifts================= */
/* ==U can add 1 or more Shifts and read val in serial if val changed== */
#include "main.h"

void setup(){
	Serial.begin(115200);
	while (!Serial){ }

	pinMode(ploadPin, OUTPUT);
	pinMode(clockEnablePin, OUTPUT);
	pinMode(dataPin, INPUT);
	pinMode(clockPin, OUTPUT);
}

void loop(){
	read();	//Read data
	checkData(); //Checking data for changes
	displayValues(); //Send to serial	
}
