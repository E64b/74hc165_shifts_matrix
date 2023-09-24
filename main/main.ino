ShiftIn<8> shift; //Количество микросхем 74hc165 

void setup()
{
	Serial.begin(9600);
	// Задаем пины: pLoadPin, clockEnablePin, dataPin, clockPin
	shift.begin(8, 9, 11, 12);
}

void displayValues()
{
	// Вывод всех битов, 1 микросхема 8 бит
	for (int i = 0; i < shift.getDataWidth(); i++)
		Serial.print(shift.state(i)); // Выводим конкретную кнопку
	Serial.println();
}

void loop()
{
	/*Читаем все кнопки, выводим если изменились*/
	if (shift.update())
		displayValues();
	delay(1);
}