#include "main.h"

uint8_t SHIFT[SHIFTS];
uint8_t OLD_SHIFT[SHIFTS];
bool update;

/* ==Reading the data stream== */
void read(){
	
	for (uint16_t m = 0; m < SHIFTS; m++)
	{		
		uint8_t CurrentShift = 0;
		delay(DEAD_TIME);
		digitalWrite(clockEnablePin, HIGH);
		digitalWrite(ploadPin, LOW);
		delayMicroseconds(pulseWidth);
		digitalWrite(ploadPin, HIGH);
		digitalWrite(clockEnablePin, LOW);	
		
		for (uint16_t i = 0; i < ALL_DATA; i++)
		{
			uint8_t value = digitalRead(dataPin);
			CurrentShift |= (value << ((ALL_DATA - 1) - i));
			digitalWrite(clockPin, HIGH);
			delayMicroseconds(pulseWidth);
			digitalWrite(clockPin, LOW);
		}	
		OLD_SHIFT[m] = SHIFT[m];
		SHIFT[m] = CurrentShift;
	}
}

/* ==Checking if the array has changed== */
void checkData(){
	for (uint8_t i = 0; i < SHIFTS; i++){
		if (OLD_SHIFT[i] != SHIFT[i]){
			update = true;
			break;
		}
	}
}

/* ==If val edit, send array== */
void displayValues(){
	if (update){
		for (uint16_t i = 0; i < SHIFTS; i++){
		/*TODO add output form*/
			Serial.print(SHIFT[i], HEX);
			Serial.print(' ');
		}
		Serial.println();
		update = false;
	}
}