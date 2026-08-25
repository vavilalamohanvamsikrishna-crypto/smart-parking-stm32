#include "lcd_i2c.h"

extern I2C_HandleTypeDef hi2c1;


/* LCD commands */

#define LCD_BACKLIGHT 0x08
#define LCD_ENABLE    0x04
#define LCD_RS        0x01


static void LCD_Write4Bits(uint8_t data)
{
    uint8_t buffer[1];

    buffer[0] = data | LCD_BACKLIGHT;

    HAL_I2C_Master_Transmit(
        &hi2c1,
        LCD_I2C_ADDRESS,
        buffer,
        1,
        HAL_MAX_DELAY
    );

    buffer[0] = data | LCD_BACKLIGHT | LCD_ENABLE;

    HAL_I2C_Master_Transmit(
        &hi2c1,
        LCD_I2C_ADDRESS,
        buffer,
        1,
        HAL_MAX_DELAY
    );

    HAL_Delay(1);

    buffer[0] = data | LCD_BACKLIGHT;

    HAL_I2C_Master_Transmit(
        &hi2c1,
        LCD_I2C_ADDRESS,
        buffer,
        1,
        HAL_MAX_DELAY
    );
}


static void LCD_SendCommand(uint8_t command)
{
    uint8_t high;
    uint8_t low;

    high = command & 0xF0;
    low = (command << 4) & 0xF0;

    LCD_Write4Bits(high);
    LCD_Write4Bits(low);
}


static void LCD_SendData(uint8_t data)
{
    uint8_t high;
    uint8_t low;

    high = data & 0xF0;
    low = (data << 4) & 0xF0;

    LCD_Write4Bits(high | LCD_RS);
    LCD_Write4Bits(low | LCD_RS);
}


void LCD_Init(void)
{
    HAL_Delay(50);

    LCD_Write4Bits(0x30);
    HAL_Delay(5);

    LCD_Write4Bits(0x30);
    HAL_Delay(1);

    LCD_Write4Bits(0x30);
    HAL_Delay(10);

    LCD_Write4Bits(0x20);
    HAL_Delay(10);

    LCD_SendCommand(0x28);
    LCD_SendCommand(0x08);

    LCD_SendCommand(0x01);
    HAL_Delay(2);

    LCD_SendCommand(0x06);
    LCD_SendCommand(0x0C);
}


void LCD_Clear(void)
{
    LCD_SendCommand(0x01);
    HAL_Delay(2);
}


void LCD_SetCursor(uint8_t row, uint8_t column)
{
    uint8_t address;

    if (row == 0)
        address = 0x80 + column;
    else
        address = 0xC0 + column;

    LCD_SendCommand(address);
}


void LCD_Print(char *str)
{
    while (*str)
    {
        LCD_SendData(*str);
        str++;
    }
}
