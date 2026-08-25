#include "parking.h"
#include "lcd_i2c.h"

extern TIM_HandleTypeDef htim1;

/* Parking status */
FloorStatus floor1 = {SLOT_FREE, SLOT_FREE};
FloorStatus floor2 = {SLOT_FREE, SLOT_FREE};


/*
 * IR SENSOR LOGIC
 *
 * Most IR obstacle sensors give:
 * LOW  = object/car detected
 * HIGH = no object
 *
 * If your sensor works opposite, change SENSOR_ACTIVE
 * from GPIO_PIN_RESET to GPIO_PIN_SET.
 */

#define SENSOR_ACTIVE GPIO_PIN_RESET


void Parking_Init(void)
{
    floor1.slot1 = SLOT_FREE;
    floor1.slot2 = SLOT_FREE;

    floor2.slot1 = SLOT_FREE;
    floor2.slot2 = SLOT_FREE;

    Gate1_Close();
    Gate2_Close();

    LCD_Clear();
    LCD_SetCursor(0, 0);
    LCD_Print("SMART PARKING");
    LCD_SetCursor(0, 1);
    LCD_Print("SYSTEM READY");

    HAL_Delay(2000);
}


void Gate1_Open(void)
{
    /*
     * Servo 1 connected to TIM1 CH1.
     *
     * Timer must be configured so that:
     * 1000 = approximately 1 ms
     * 1500 = approximately 1.5 ms
     * 2000 = approximately 2 ms
     */

    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 2000);
}


void Gate1_Close(void)
{
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 1000);
}


void Gate2_Open(void)
{
    /*
     * Change this channel if your CubeMX configuration
     * uses a different timer channel for Servo 2.
     */

    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 2000);
}


void Gate2_Close(void)
{
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 1000);
}


uint8_t Parking_GetFreeSlots(void)
{
    uint8_t freeSlots = 0;

    if (floor1.slot1 == SLOT_FREE)
        freeSlots++;

    if (floor1.slot2 == SLOT_FREE)
        freeSlots++;

    if (floor2.slot1 == SLOT_FREE)
        freeSlots++;

    if (floor2.slot2 == SLOT_FREE)
        freeSlots++;

    return freeSlots;
}


uint8_t Parking_IsFull(void)
{
    if ((floor1.slot1 == SLOT_OCCUPIED) &&
        (floor1.slot2 == SLOT_OCCUPIED) &&
        (floor2.slot1 == SLOT_OCCUPIED) &&
        (floor2.slot2 == SLOT_OCCUPIED))
    {
        return 1;
    }

    return 0;
}


void Parking_Update(void)
{
    /* Read Floor 1 slots */

    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == SENSOR_ACTIVE)
        floor1.slot1 = SLOT_OCCUPIED;
    else
        floor1.slot1 = SLOT_FREE;


    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) == SENSOR_ACTIVE)
        floor1.slot2 = SLOT_OCCUPIED;
    else
        floor1.slot2 = SLOT_FREE;


    /* Read Floor 2 slots */

    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2) == SENSOR_ACTIVE)
        floor2.slot1 = SLOT_OCCUPIED;
    else
        floor2.slot1 = SLOT_FREE;


    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3) == SENSOR_ACTIVE)
        floor2.slot2 = SLOT_OCCUPIED;
    else
        floor2.slot2 = SLOT_FREE;


    /*
     * Update LCD
     */

    LCD_Clear();

    LCD_SetCursor(0, 0);

    if (floor1.slot1 == SLOT_FREE)
        LCD_Print("F1:S1 Free ");
    else
        LCD_Print("F1:S1 Full ");


    LCD_SetCursor(0, 1);

    if (floor1.slot2 == SLOT_FREE)
        LCD_Print("S2 Free");
    else
        LCD_Print("S2 Full");


    HAL_Delay(1000);


    LCD_Clear();

    LCD_SetCursor(0, 0);

    if (floor2.slot1 == SLOT_FREE)
        LCD_Print("F2:S1 Free ");
    else
        LCD_Print("F2:S1 Full ");


    LCD_SetCursor(0, 1);

    if (floor2.slot2 == SLOT_FREE)
        LCD_Print("S2 Free");
    else
        LCD_Print("S2 Full");


    HAL_Delay(1000);


    /*
     * Gate 1
     *
     * If a car is detected at Gate 1 and
     * Floor 1 has at least one free slot,
     * open Gate 1.
     */

    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4) == SENSOR_ACTIVE)
    {
        if ((floor1.slot1 == SLOT_FREE) ||
            (floor1.slot2 == SLOT_FREE))
        {
            Gate1_Open();
            HAL_Delay(3000);
            Gate1_Close();
        }
    }


    /*
     * Gate 2
     */

    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5) == SENSOR_ACTIVE)
    {
        if ((floor2.slot1 == SLOT_FREE) ||
            (floor2.slot2 == SLOT_FREE))
        {
            Gate2_Open();
            HAL_Delay(3000);
            Gate2_Close();
        }
    }


    /*
     * If all four slots are occupied
     */

    if (Parking_IsFull())
    {
        LCD_Clear();

        LCD_SetCursor(0, 0);
        LCD_Print("PARKING FULL");

        LCD_SetCursor(0, 1);
        LCD_Print("NO SPACE");

        HAL_Delay(2000);
    }
}
