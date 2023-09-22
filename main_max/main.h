#pragma once

#include <Arduino.h>
#include <stdio.h>
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

extern bool update; //Flag for updating
extern uint8_t Shift[shifts]; //Array for register
extern uint8_t OldShift[shifts]; //Old Array

void read();
void checkData();
void displayValues();

#pragma pack(pop)