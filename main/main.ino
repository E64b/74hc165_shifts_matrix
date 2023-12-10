/* =================Unlimited Shifts================= */
/* ==U can add 1 or more Shifts and read val in serial if val changed== */
#include "main.h"

SoftwareSerial softSerial(RX, TX);

void setup() {
  Serial.begin(115200);
  softSerial.begin(115200);
  while (!Serial) { }
  //Serial.println("Serial OK");
  pinMode(RX, INPUT);
  pinMode(TX, OUTPUT);
  pinMode(ploadPin, OUTPUT);
  pinMode(dataPin, INPUT);
  pinMode(clockPin, OUTPUT);

  initSendBuffer();
}

void loop() {
  softSer();
  read();
  checkData();
  displayValues();
  sendValues();
  Serial_In();
  Serial_Out();
}
