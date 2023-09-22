/* =================Unlimited Shifts================= */
/* ==U can add 1 or more Shifts and read val in serial if val changed== */

/* ==Number of chips 74hc165== */
#define shifts 54

/* ==pins== */
#define ploadPin 8
#define clockEnablePin 9
#define dataPin 11
#define clockPin 12

/* == Data length of a single chip == */
#define SINGLE_CHIP_DATA_LENGTH 8

/* ==Total length of data== */
#define all_data (SINGLE_CHIP_DATA_LENGTH * shifts)

/* ==Pulse length== */
#define pulseWidth 5

bool update; //Flag for updating
uint8_t Shift[shifts] = {}; //Array for register
uint8_t OldShift[shifts]= {}; //Old Array

void setup(){
	Serial.begin(115200);
	while (!Serial);
	pinMode(ploadPin, OUTPUT); 
	pinMode(clockEnablePin, OUTPUT);
	pinMode(dataPin, INPUT);
	pinMode(clockPin, OUTPUT);
}

/* ==Reading the data stream== */
void read(){
	for (int m = 0; m < shifts; m++){
		uint8_t CurrentShift = 0;
		//TODO добавить проверку получения не всего сообщения
		digitalWrite(clockEnablePin, HIGH);
		digitalWrite(ploadPin, LOW);
		delayMicroseconds(pulseWidth);
		digitalWrite(ploadPin, HIGH);
		digitalWrite(clockEnablePin, LOW);

		for (int i = 0; i < data; i++){
			//TODO добавить проверку получения < 8 бит
			uint8_t value = digitalRead(dataPin);
			CurrentShift |= (value << ((data - 1) - i));
			digitalWrite(clockPin, HIGH);
			delayMicroseconds(pulseWidth);
			digitalWrite(clockPin, LOW);
		}		
		OldShift[m] = Shift[m];
		Shift[m] = CurrentShift;
	}
}

/* ==Checking if the array has changed== */
void checkData(){
	for (int i = 0; i < shifts; i++){
		if (OldShift[i] != Shift[i]){
			update = true;
			break;
		}
	}
}

/* ==If val edit, send array== */
void displayValues(){	
	if (update){
		for (int i = 0; i < shifts; i++){
		Serial.printl(Shift[i], HEX); //Send current val HEX
		Serial.print(' ');
		}
		Serial.println();
		update = false;
	}

/* Тот самый пример

	if (!update) {
		return;
	}

	update = false;
	for (int i = 0; i < shifts; i++)
	{
		Serial.printl(Shift[i], HEX); //Send current val HEX
		Serial.print(' ');
	}
	Serial.println();
*/
}

void loop(){
	read();	//Read data
	checkData(); //Checking data for changes
	displayValues(); //Send to serial	
	delay(10);
}
