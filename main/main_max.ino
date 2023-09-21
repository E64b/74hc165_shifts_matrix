#include <SPI.h>

/*Колличество микросхем 74hc165*/
#define shifts 54

/*Задаем pins*/
#define ploadPin 8
#define clockEnablePin 9
#define dataPin 11
#define clockPin 12

/*Длинна данных*/
#define data 8

/*Настройки импульсов*/
#define pulseWidth 5

bool update; //Флаг на обновление
uint8_t CurrentShift; //Текущий регистр
uint8_t Shift[shifts] = {}; //Массив для регистра

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
	/*Выводим массив если изменился*/
	if (update() == true)
	{
		for (int i = 0; i < shifts; i++)
		{
		Serial.printl(Shift[i], HEX); // Выводим конкретную кнопку
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
