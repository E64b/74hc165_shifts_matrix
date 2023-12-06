/* =================Unlimited Shifts================= */
/* ==U can add 1 or more Shifts and read val in serial if val changed== */
#include "main.h"

void setup() {
  Serial.begin(115200);
  while (!Serial) {
  }
  Serial.println("Serial OK");
  pinMode(ploadPin, OUTPUT);
  pinMode(dataPin, INPUT);
  pinMode(clockPin, OUTPUT);

  initSendBuffer();
}

void loop() {
  read();          // Read data
  checkData();     // Checking data for changes
  displayValues(); // Send to serial
  sendValues();
  Serial_In();
  Serial_Out();
}
