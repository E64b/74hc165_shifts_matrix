#include "main.h"
/* ==Reading the data stream== */
void read()
{
	for (int m = 0; m < shifts; m++)
	{
		uint8_t CurrentShift = 0;
		//TODO add a check for the arrival of not the entire message
		digitalWrite(clockEnablePin, HIGH);
		digitalWrite(ploadPin, LOW);
		delayMicroseconds(pulseWidth);
		digitalWrite(ploadPin, HIGH);
		digitalWrite(clockEnablePin, LOW);

		for (int i = 0; i < SINGLE_CHIP_DATA_LENGTH; i++)
		{
			//TODO add a bit skip check
			uint8_t value = digitalRead(dataPin);
			CurrentShift |= (value << ((SINGLE_CHIP_DATA_LENGTH - 1) - i));
			digitalWrite(clockPin, HIGH);
			delayMicroseconds(pulseWidth);
			digitalWrite(clockPin, LOW);
		}
		OldShift[m] = Shift[m];
		Shift[m] = CurrentShift;
	}
}

/* ==Checking if the array has changed== */
void checkData()
{
	for (int i = 0; i < shifts; i++)
	{
		if (OldShift[i] != Shift[i])
		{
			update = true;
			break;
		}
	}
}

/* ==If val edit, send array== */
void displayValues()
{
	if (update)
	{
		for (int i = 0; i < shifts; i++)
		{
			Serial.println(Shift[i], HEX); //Send current val HEX
			Serial.print(' ');
		}
		Serial.println();
		update = false;
	}
}