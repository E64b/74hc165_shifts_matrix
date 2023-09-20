#include "ShiftIn.h"

ShiftIn<8> shift; //Колличество микросхем 74hc165 

void setup() {
	Serial.begin(9600);
	// Задаем пины: pLoadPin, clockEnablePin, dataPin, clockPin
	shift.begin(8, 9, 11, 12);
}

void displayValues() {
	// Вывод всех битов, 1 микросхема 8 бит
	for(int i = 0; i < shift.getDataWidth(); i++)
		Serial.print( shift.state(i) ); // Выводим конктертную кнопку
	Serial.println();
}

void loop() {
	if(shift.update()) // Читаем все кнопки, выводим если изменились
		displayValues();
	delay(1);
}
