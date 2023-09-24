#include "main.h"

uint8_t SHIFT[SHIFTS];
uint8_t OLD_SHIFT[SHIFTS];
bool update;

/* ==Reading the data stream== */
void read(){
	digitalWrite(clockEnablePin, HIGH);
	digitalWrite(ploadPin, LOW);
	delayMicroseconds(pulseWidth);
	digitalWrite(ploadPin, HIGH);
	digitalWrite(clockEnablePin, LOW);
	for (uint16_t m = 0; m < SHIFTS; m++)
	{
	    delay(DEAD_TIME);
	    uint8_t CurrentShift = 0;
		
		for (uint16_t i = 0; i < SINGLE_CHIP_DATA_LENGTH; i++)
		{
			uint8_t value = digitalRead(dataPin);
			CurrentShift |= (value << ((SINGLE_CHIP_DATA_LENGTH - 1) - i));
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
void displayValues()
{
	if (update){
		for (byte i = 0; i < SHIFTS; i++){
		/*TODO add output form*/
			Serial.print(SHIFT[i], BIN);
			Serial.print(' ');
		}
		Serial.println();
		update = false;    
	}
}
