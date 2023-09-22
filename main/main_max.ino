/* =================Unlimited Shifts================= */
/* ==U can add 1 or more Shifts and read val in serial if val changed== */


// ------------------------------------------------------------------------------------------
// Code style
// - константы и define принято (в кругах программистов, не ардуинщиков и прочих людей, 
//   которые решили, что они боги в этом деле) принято 
//   называть в SCREAMING_SNAKE_CASE - это дает тебе визуальное понимание при чтении кода, 
//   что это константа
// - используй говорящие имена СРАЗУ, а не обвешивай комментами
//   те вместо #define data 8 // Data length of a single chip
//   используй #define SINGLE_CHIP_DATA_LENGTH 8 
//   не экономь на буквах - они бесплатные ))
// - старайся придерживаться одного формата названия переменных, а то у тебя то PascalCase, 
//   то camelCase, то snake_case - читается плохо
//   например bool update; 
//   но почему то сразу uint8_t CurrentShift; // почему формат имени поменялся?
// - переменные принято называть в camelCase (в подавляющем количестве языков), 
//   методы/функции все либо PascalCase (стандарт C) либо все camelCase (стандарт явы?)
// - используй пробелы вместо табов - код будет выглядеть одинаково у всех (holywar, блеать)
// ------------------------------------------------------------------------------------------

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
uint8_t CurrentShift; //Current register
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
		uint8_t result = 0;

		digitalWrite(clockEnablePin, HIGH);
		digitalWrite(ploadPin, LOW);
		delayMicroseconds(pulseWidth);
		digitalWrite(ploadPin, HIGH);
		digitalWrite(clockEnablePin, LOW);

		for (int i = 0; i < data; i++){
			uint8_t value = digitalRead(dataPin); // ShiftType нафиг тут не нужен - фигачишь обычными uint8_t и ебись оно все конем //DONE
			result |= (value << ((data - 1) - i));
			digitalWrite(clockPin, HIGH);
			delayMicroseconds(pulseWidth);
			digitalWrite(clockPin, LOW);
		}
		Shift[m] = result;
		OldShift[m] = Shift[m]; // <-- эмммм... Те сперва кладешь новое значение в буфер, а потом пытаешься его переложить еще и как старое? А зачем? Может ты сперва хотел бы сохранить текущее значение, а потом его обновить?
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

/*If val edit, send array*/
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
