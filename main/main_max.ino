/* =================Unlimit Shifts================= */
/* ==U can add 1 or more Shifts and read val in serial if val changed== */

/* ==Number of chips 74hc165== */
#define shifts 54

/* ==pins== */
#define ploadPin 8
#define clockEnablePin 9
#define dataPin 11
#define clockPin 12

/* == Data length of a single chip == */
#define data 8

/* ==Total length of data== */
#define all_data (data * shifts)

/* ==Pulse length== */
#define pulseWidth 5

bool update; //Flag for updating
uint8_t CurrentShift; //Current register
uint8_t Shift[shifts] = {}; //Array for register

void setup(){
	Serial.begin(115200);
	while (!Serial){	}
	pinMode(ploadPin, OUTPUT);
	pinMode(clockEnablePin, OUTPUT);
	pinMode(dataPin, INPUT);
	pinMode(clockPin, OUTPUT);
}

void read()
{
	digitalWrite(clockEnablePin, HIGH);
	digitalWrite(ploadPin, LOW);
	delayMicroseconds(pulseWidth);
	digitalWrite(ploadPin, HIGH);
	digitalWrite(clockEnablePin, LOW);

	for (uint32_t m = 0; m < shifts; m++)
	{
		for (uint8_t i = 0; i < data; i++)
		{
			ShiftType value = digitalRead(dataPin);
			result |= (value << ((data - 1) - i));
			digitalWrite(clockPin, HIGH);
			delayMicroseconds(pulseWidth);
			digitalWrite(clockPin, LOW);
			return result;
		}
		Shift[m] = result;
	}
}

void displayValues()
{
	/*If val edit, send array*/
	if (update() == true)
	{
		for (int i = 0; i < shifts; i++)
		{
		Serial.printl(Shift[i], HEX); //Send current val HEX
		Serial.print(' ');
		}
		Serial.println();
		update = false;
	}
}

void loop()
{
	read();	//Read data
	displayValues(); //Send to serial	
	delay(1);
}
