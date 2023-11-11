#include "main.h"

uint8_t sendBuffer[7 + SHIFTS];
uint8_t *SHIFT;
uint8_t OLD_SHIFT[SHIFTS];
bool update;

uint8_t CRC8(uint8_t *data, int length) {
  uint8_t crc = 0x00;
  uint8_t extract;
  uint8_t sum;
  for (int i = 0; i < length; i++) {
    extract = *data;
    for (uint8_t tempI = 8; tempI; tempI--) {
      sum = (crc ^ extract) & 0x01;
      crc >>= 1;
      if (sum) {
        crc ^= 0x8C;
      }
      extract >>= 1;
    }
    data++;
  }
  return crc;
}

/* ==Reading the data stream== */
void read() {
  delay(DEAD_TIME);
  digitalWrite(ploadPin, LOW);
  delayMicroseconds(pulseWidth);
  digitalWrite(ploadPin, HIGH);
  for (uint16_t m = 0; m < SHIFTS; m++) {
    uint8_t CurrentShift = 0;

    for (uint16_t i = 0; i < SINGLE_CHIP_DATA_LENGTH; i++) {
      uint8_t value = digitalRead(dataPin);
      CurrentShift |= (value << ((SINGLE_CHIP_DATA_LENGTH - 1) - i));
      digitalWrite(clockPin, HIGH);
      delayMicroseconds(pulseWidth);
      digitalWrite(clockPin, LOW);
    }

    OLD_SHIFT[m] = SHIFT[m];
    SHIFT[m] = CurrentShift;
  }
}

/* ==Checking if the array has changed== */
void checkData() {
  for (uint8_t i = 0; i < SHIFTS; i++) {
    if (OLD_SHIFT[i] != SHIFT[i]) {
      update = true;
      break;
    }
  }
}

/* ==If val edit, send array== */
void displayValues() {
  if (!update) {
    return;
  }
  for (uint16_t i = 0; i < SHIFTS; i++) {
    Serial.print(SHIFT[i], BIN);
    Serial.print(' ');
  }
  Serial.println();
  update = false;
}

/* ==If val edit, send array== */
void sendValues() {
  if (!update) {
    return;
  }

  uint8_t crc = CRC8(sendBuffer, sizeof(sendBuffer) - 1);
  sendBuffer[sizeof(sendBuffer) - 1] = crc;
  Serial.write(sendBuffer, (sizeof(sendBuffer)));
  update = false;
}

void initSendBuffer() {
  sendBuffer[0] = 0x04;
  sendBuffer[1] = 0x11;
  sendBuffer[2] = 0x00;
  sendBuffer[3] = DEVICE_ID;
  sendBuffer[4] = (SHIFTS >> 8) & 0xFF;
  sendBuffer[5] = SHIFTS & 0xFF;
  SHIFT = sendBuffer + 6;
}
