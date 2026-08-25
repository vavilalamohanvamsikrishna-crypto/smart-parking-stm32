
#ifndef PARKING_H
#define PARKING_H

#include "main.h"

#define SLOT_FREE       0
#define SLOT_OCCUPIED   1

typedef struct
{
    uint8_t slot1;
    uint8_t slot2;
} FloorStatus;

extern FloorStatus floor1;
extern FloorStatus floor2;

void Parking_Init(void);
void Parking_Update(void);

uint8_t Parking_GetFreeSlots(void);
uint8_t Parking_IsFull(void);

void Gate1_Open(void);
void Gate1_Close(void);

void Gate2_Open(void);
void Gate2_Close(void);

#endif
