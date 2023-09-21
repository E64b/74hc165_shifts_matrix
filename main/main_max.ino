#include <SPI.h>

/*����������� ��������� 74hc165*/
#define shifts 54

/*������ pins*/
#define ploadPin 8
#define clockEnablePin 9
#define dataPin 11
#define clockPin 12

/*������ ������*/
#define data 8

/*��������� ���������*/
#define pulseWidth 5

bool update; //���� �� ����������
uint8_t CurrentShift; //������� �������
uint8_t Shift[shifts] = {}; //������ ��� ��������

void setup(){
	Serial.begin(115200);
	while (!Serial){
	}
	pinMode(ploadPin, OUTPUT);
	pinMode(clockEnablePin, OUTPUT);
	pinMode(dataPin, INPUT);
	pinMode(clockPin, OUTPUT);
}

void read()
{
	ShiftType result = 0;
	digitalWrite(clockEnablePin, HIGH);
	digitalWrite(ploadPin, LOW);
	delayMicroseconds(pulseWidth);
	digitalWrite(ploadPin, HIGH);
	digitalWrite(clockEnablePin, LOW);

	for (uint8_t i = 0; i < data; i++)
	{
		ShiftType value = digitalRead(dataPin);
		result |= (value << ((data - 1) - i));
		digitalWrite(clockPin, HIGH);
		delayMicroseconds(pulseWidth);
		digitalWrite(clockPin, LOW);
	}
	currentState = result;
	return result;
}

void displayValues()
{
	/*������� ������ ���� ���������*/
	if (update() == true)
	{
		for (int i = 0; i < shifts; i++)
		{
		Serial.printl(Shift[i], HEX); // ������� ���������� ������
		Serial.print(' ');
		}
		Serial.println();
		update = false;
	}
}

void loop()
{
	read();	
	displayValues();		
	delay(1);
}
