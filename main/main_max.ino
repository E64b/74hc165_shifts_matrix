#include "Shiftln_max.h"
#include <SPI.h>

ShiftIn<8> shift; //���������� ��������� 74hc165 

void setup()
{
	Serial.begin(9600);
	// ������ ����: pLoadPin, clockEnablePin, dataPin, clockPin
	shift.begin(8, 9, 11, 12);
}

void displayValues()
{
	// ����� ���� �����, 1 ���������� 8 ���
	for (int i = 0; i < shift.getDataWidth(); i++)
		Serial.print(shift.state(i)); // ������� ���������� ������
	Serial.println();
}

void loop()
{
	/*������ ��� ������, ������� ���� ����������*/
	if (shift.update())
		displayValues();
	delay(1);
}
