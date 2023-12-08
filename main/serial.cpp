#include "main.h"

#define BUFF_SIZE 256

uint8_t sendBuffer[7 + SHIFTS];
uint8_t *SHIFT;
uint8_t OLD_SHIFT[SHIFTS];
bool update;
bool pong = false;

uint16_t buffIndex = 0;
uint8_t buff[BUFF_SIZE];

MessageAPI *pMsg = NULL;

#define STATE_HS_UNKNOWN 0
#define STATE_HS_HEADER 1
#define STATE_HS_ID 2
#define STATE_HS_SRC 3
#define STATE_HS_PROTO 4
#define STATE_HS_FOOTER 5
#define STATE_PKG_READING_HEADER 6
#define STATE_PKG_READING_DATA 7
#define STATE_PKG_READING_CRC 8

uint8_t hsState = STATE_HS_UNKNOWN;
String hsId;
String hsProto;

void Serial_In() {
  while (Serial.available() > 0) {
    char val = (char)Serial.read();
    softSerial.print(val);
    
checkState:
    switch (hsState) {
      case STATE_HS_UNKNOWN:
        if (val == '>') {
          hsState = STATE_HS_HEADER;
        } else {
          if (buffIndex == 0 && val == 0x04 || buffIndex == 1 && val == 0x11) {
            buff[buffIndex++] = val;
            if (buffIndex == 2) {
              hsState = STATE_PKG_READING_HEADER;
            }
          } else {
            buffIndex = 0;
          }
        }
        break;

      case STATE_HS_HEADER:
        if (val == '>') {
          hsId = "";
          hsState = STATE_HS_ID;
          buffIndex = 0;
          pMsg = NULL;
        } else {
          hsState = STATE_HS_UNKNOWN;
          buff[buffIndex++] = '>';
          buff[buffIndex++] = val;
        }
        break;

      case STATE_HS_ID:
        if (val == ':') {
          hsState = STATE_HS_SRC;
        } else if (val >= 'A' && val <= 'F' || val >= 'a' && val <= 'f' || val >= '0' && val <= '9' || val == '.' || val == '-') {
          hsId += val;
        } else {
          hsState = STATE_HS_UNKNOWN;
        }
        break;

      case STATE_HS_SRC:
        if (val == ':') {
          hsProto = "";
          hsState = STATE_HS_PROTO;
        } else if (val != '0') {
          hsState = STATE_HS_UNKNOWN;
        }
        break;

      case STATE_HS_PROTO:
        if (val == '<') {
          hsState = STATE_HS_FOOTER;
        } else {
          hsProto += val;
        }
        break;

      case STATE_HS_FOOTER:
        if (val == '<') {
          if (hsProto == "nlic-v1") {
            String str("<<");
            str += hsId + ":" + String(DEVICE_ID, HEX) + ":1:" + FEATURE_ID;
            str += ">>";
            Serial.print(str);
          }
        }
        hsState = STATE_HS_UNKNOWN;
        break;

      case STATE_PKG_READING_HEADER:
        if (buffIndex >= sizeof(MessageHeader)) {
          pMsg = (MessageAPI *)buff;
          if (pMsg->header.size <= 0x7fff && pMsg->header.target == DEVICE_ID) {
            if (pMsg->header.size == 0) {
              hsState = STATE_PKG_READING_CRC;
              goto checkState;
            } else {
              hsState = STATE_PKG_READING_DATA;
              goto checkState;
            }
          } else {
            hsState = STATE_HS_UNKNOWN;
            buffIndex = 0;
            pMsg = NULL;
          }
        } else {
          buff[buffIndex++] = val;
        }
        break;

      case STATE_PKG_READING_DATA:
        if (buffIndex >= pMsg->header.size - 1) {
          hsState = STATE_PKG_READING_CRC;
          goto checkState;
        } else {
          buff[buffIndex++] = val;
        }
        break;

      case STATE_PKG_READING_CRC:
        if (pMsg && pMsg->header.size <= buffIndex) {
          uint8_t crc = pMsg->CRC8();
        }
        buffIndex = 0;
        hsState = STATE_HS_UNKNOWN;
        pMsg = NULL;
        break;
    }
  }
}

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

/* ==If val edit, send array== */
void displayValues() {
  if (DEBUG) {
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

void Serial_Out() {
  if (pong) {
    PongMessage msg;
    msg.Send();
    pong = false;
  }
}
