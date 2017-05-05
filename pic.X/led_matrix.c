/* 
 * File:   led_matrix.c
 * Author: gchinellato
 *
 * Created on April 26, 2017, 1:12 AM
 */

#include "led_matrix.h"
#include "spi.h"

void LED_Matrix_Init(void)
{
    // test mode on
    SPI_Read_Write(SS_0, DISPLAY_TEST, 0x01); 
    __delay_ms(100); 
    // test mode off
    SPI_Read_Write(SS_0, DISPLAY_TEST, 0x00); 
    // use lowest intensity
    SPI_Read_Write(SS_0, INTENSITY, 0x00);
    // scan all digits
    SPI_Read_Write(SS_0, SCAN_LIMIT, 0x07);
     //disable BCD mode
    SPI_Read_Write(SS_0, DECODE_MODE, 0x00);
    // turn on chip
    SPI_Read_Write(SS_0, SHUTDOWN, 0x01); 
}

void LED_Matrix_Clear(void)
{ 
    char i;
    for(i=1; i <= LED_MATRIX_ROW; i++)
    {
        SPI_Read_Write(SS_0, i, 0);
    }
}

void LED_Matrix_Update(char x, char y)
{
    SPI_Read_Write(SS_0, x, (1 << y));
}

