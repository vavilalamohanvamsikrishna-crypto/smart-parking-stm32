#ifndef LCD_I2C_H
#define LCD_I2C_H

#include "main.h"

#define LCD_I2C_ADDRESS (0x27 << 1)

void LCD_Init(void);
void LCD_Clear(void);
void LCD_SetCursor(uint8_t row, uint8_t column);
void LCD_Print(char *str);

#endif
