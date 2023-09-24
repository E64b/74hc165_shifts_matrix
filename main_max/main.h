#pragma once

#include <Arduino.h>

/* ==Number of chips 74hc165== */
#define SHIFTS 8 //<<<<<<<<< ����� ����� �� 1 �� 99

/* ==pins== */
#define ploadPin 8
#define clockEnablePin 9
#define dataPin 11
#define clockPin 12

/* == Data length of a single chip == */
#define SINGLE_CHIP_DATA_LENGTH 8

/* ==Total length of data== */
#define ALL_DATA (SINGLE_CHIP_DATA_LENGTH * SHIFTS)

/* ==Pulse length== */
#define pulseWidth 6
#define DEAD_TIME 1

extern bool update; //Flag for updating
extern uint8_t SHIFT[SHIFTS]; //Array for register
extern uint8_t OLD_SHIFT[SHIFTS]; //Old Array

void read();
void checkData();
void displayValues();
