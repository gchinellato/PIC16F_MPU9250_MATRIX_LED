/*
 * File:   main.c
 * Author: gchinellato
 *
 * Created on April 24, 2017, 10:59 PM
 */

#include <stdio.h>
#include <math.h>
#include "config.h"
#include "spi.h"
#include "serial.h"
#include "led_matrix.h"
#include "imu.h"

#define LP 10

char ledInit[] = {
    0b11111111,
    0b11111111,
    0b11000011,
    0b11011011,
    0b11011011,
    0b11000011,
    0b11111111,
    0b11111111,
};

void main(void) 
{          
    float pitch, roll;
    float factor_x, factor_j;
    char x,y;
    char i,j;
    char col, row;
    char lastRow;  
    unsigned int timer = 0;
    
    //Init UART
    USART_Init();
    printf("USART_Init OK\r\n"); 
    
    //Init SPI
    SPI_Init();
    printf("SPI_Init OK\r\n"); 
    
    //Init LED Matrix and MAX7219
    LED_Matrix_Init();
    printf("LED_Matrix_Init OK\r\n"); 
 
    //IMU Init
    IMU_Init();  
    printf("IMU_Init OK\r\n"); 
    printf("IMU WHO AM I:%x\r\n", IMU_WhoAmI()); 
    
    LED_Matrix_Clear_All();    
   
    while(1)
    { 
        accel accel;
        IMU_Accel_Read(&accel);
        gyro gyro;
        IMU_Gyro_Read(&gyro);   

        roll = atan2(accel.y, accel.z);
        pitch = atan2(accel.x, accel.z); 
        //printf("roll: %f pitch: %f\r\n", roll*RAD_TO_DEG, pitch*RAD_TO_DEG); 

        factor_x = ((roll*RAD_TO_DEG)-(-90.0))/(90.0-(-90.0));
        factor_j = ((pitch*RAD_TO_DEG)-(-90.0))/(90.0-(-90.0));        
        i=factor_x*(8-1)+1; 
        j=factor_j*(7-0)+0; 

        for(x=1; x <= LED_MATRIX_COL; x++)
        {
            col = x;
            row = ledInit[x-1];
            if(x == i)
            {
                //led to move at x,y position
                row &= (~(1 << j));                 
                
                //blink each 20ms, if it is the same value as before
                if(timer == (LP*2))
                {
                    if(row == lastRow)  
                    {
                        row ^= (1 << j);
                    }
                    timer=0;
                }
                lastRow = row;
            }
            LED_Matrix_Update(col,row);
        }        
        __delay_ms(LP);
        timer += 1;
    }
    return;
}