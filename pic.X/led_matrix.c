/* 
 * File:   led_matrix.c
 * Author: gchinellato
 *
 * Created on April 26, 2017, 1:12 AM
 */

#include "led_matrix.h"
#include "spi.h"

void initLEDMatrix(void)
{ 
    char i;
    for(i=1; i <= MATRIX_LED_ROW; i++)
    {
        SPI_Read_Write(SS_0, i, 0);
    }
}
