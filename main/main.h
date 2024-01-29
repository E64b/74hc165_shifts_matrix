#pragma once

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <inttypes.h>
#include <stdio.h>

#define TX 25
#define RX 26

#define DEBUG 0 // Init DEBUG mode

#define FEATURE_ID "floor-buttons"
#define DEVICE_ID 0x30

/* ==Number of chips 74hc165== */
#define SHIFTS 54

/* ==pins== */
#define ploadPin 23
#define dataPin 22
#define clockPin 21

/* == Data length of a single chip == */
#define SINGLE_CHIP_DATA_LENGTH 8

/* ==Total length of data== */
#define ALL_DATA (SINGLE_CHIP_DATA_LENGTH * SHIFTS)

/* ==Pulse length== */
#define pulseWidth 5
#define DEAD_TIME 0

struct MessageHeader {
  uint16_t magic;
  uint8_t target;
  uint8_t sender;
  uint16_t size;
};

struct MessageFooter {
  uint8_t crc;
};

#define PROTO_MAGIC 0x0411

struct MessageAPI {

  MessageHeader header;
  bool HasValidHeader() const { return header.magic == PROTO_MAGIC; }
  bool HasValidSize() const { return header.size <= 0x7fff; }
  virtual bool IsValid() const { return HasValidHeader() && HasValidSize(); }

  uint8_t CRC8() const {
    uint8_t crc = 0x00;
    uint8_t extract;
    uint8_t sum;
    const uint8_t *data = (const uint8_t *)this;
    for (int i = 0; i < GetSize() - 1; i++) {
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

  void Send() {
    MakeValid();
    Serial.write((uint8_t *)this, GetSize());
  }

protected:
  virtual void MakeValid() {
    header.magic = PROTO_MAGIC;
    header.target = 0x00;
    header.sender = DEVICE_ID;
    header.size = GetSize();
  }
  virtual uint16_t GetSize() const { return sizeof(MessageAPI) + 1; }
};

template <typename T> struct Message : public MessageAPI {
  T payload;
  MessageFooter footer;

  bool HasValidCrc() const { return footer.crc == MessageAPI::CRC8(); }
  bool IsValid() const { return MessageAPI::IsValid() && HasValidCrc(); }

protected:
  uint16_t GetSize() const {
    return sizeof(T) + sizeof(MessageHeader) + sizeof(MessageFooter);
  }

  void MakeValid() {
    MessageAPI::MakeValid();
    footer.crc = MessageAPI::CRC8();
  }
};

struct EmptyMessage : public MessageAPI {
  MessageFooter footer;

  bool HasValidCrc() const { return footer.crc == MessageAPI::CRC8(); }
  bool IsValid() const { return MessageAPI::IsValid() && HasValidCrc(); }

protected:
  uint16_t GetSize() const {
    return sizeof(MessageHeader) + sizeof(MessageFooter);
  }
  void MakeValid() {
    MessageAPI::MakeValid();
    footer.crc = MessageAPI::CRC8();
  }
};

void read();
void checkData();
void displayValues();
void sendValues();
void initSendBuffer();
void Serial_In();
void Serial_Out();
void softSer();

extern SoftwareSerial softSerial;
extern uint8_t sendBuffer[7 + SHIFTS];
extern uint8_t *SHIFT;
extern uint8_t OLD_SHIFT[SHIFTS];
extern bool update;
extern bool pong;

typedef EmptyMessage PongMessage;

#pragma pack(pop)