#include "main.h"

uint8_t SHIFT[SHIFTS];
uint8_t OLD_SHIFT[SHIFTS];
bool update;

/* ==Reading the data stream== */
void read(){
	delay(DEAD_TIME);
	digitalWrite(ploadPin, LOW);
	delayMicroseconds(pulseWidth);
	digitalWrite(ploadPin, HIGH);
	for(uint16_t m=0; m<SHIFTS; m++){
		uint8_t CurrentShift=0;

		for(uint16_t i=0; i<SINGLE_CHIP_DATA_LENGTH; i++){
			uint8_t value=digitalRead(dataPin);
			CurrentShift|=(value<<((SINGLE_CHIP_DATA_LENGTH-1)-i));
			digitalWrite(clockPin, HIGH);
			delayMicroseconds(pulseWidth);
			digitalWrite(clockPin, LOW);
			}

		OLD_SHIFT[m]=SHIFT[m];
		SHIFT[m]=CurrentShift;
		}
	}

/* ==Checking if the array has changed== */
void checkData(){
	for(uint8_t i=0; i<SHIFTS; i++){
		if(OLD_SHIFT[i]!=SHIFT[i]){
			update=true;
			break;
			}
		}
	}

/* ==If val edit, send array== */
void displayValues(){
	if(update){
		for(uint16_t i=0; i<SHIFTS; i++){
			Serial.print(SHIFT[i], BIN);
			Serial.print(' ');
			}
		Serial.println();
		update=false;
		}
	}
