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

//state machine modes
typedef enum {
    NUMERIC,
    VISUAL,
} eState;

eState state = NUMERIC;

void main(void) 
{          
    float pitch, roll;
    float factor_x, factor_j;
    char x,y;
    char i,j;
    char col, row;
    char lastRow;  
    unsigned int timerBlink = 0;
    unsigned int timerState = 0;
    
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

        //compute angles
        roll = atan2(accel.y, accel.z);
        pitch = atan2(accel.x, accel.z); 
        //printf("roll: %f pitch: %f\r\n", roll*RAD_TO_DEG, pitch*RAD_TO_DEG); 

        //interpolate pitch and roll angles according number of leds
        factor_x = ((roll*RAD_TO_DEG)-(-90.0))/(90.0-(-90.0));
        factor_j = ((pitch*RAD_TO_DEG)-(-90.0))/(90.0-(-90.0));        
        i=factor_x*(8-1)+1; 
        j=factor_j*(7-0)+0; 
        
        //update display        
        if(state == VISUAL)
        {
            //graphic position
            for(x=1; x <= LED_MATRIX_COL; x++)
            {
                col = x;
                row = ledInit_Default[x-1];
                if(x == i)
                {
                    //led to move at x,y position
                    row &= (~(1 << j));                 

                    //blink each 50ms, if it is the same value as before
                    //check LP base time
                    if(timerBlink >= (LP*5))
                    {
                        if(row == lastRow)  
                        {
                            row ^= (1 << j);
                        }
                        timerBlink=0;
                    }
                    lastRow = row;
                }
                LED_Matrix_Update(col,row);
            }   
        }   
        else if(state == NUMERIC)
        {
            //numeric value
            LED_Matrix_SetDigit(((pitch*RAD_TO_DEG*100)/100));
        }        
           
        //state mahcine, change state each 2s
        if(timerState >= (LP*200))
        {
            switch(state)
            {
                case NUMERIC:
                    state = VISUAL;
                    LED_Matrix_Clear_All(); 
                    timerState=0;
                    break;
                case VISUAL:
                    state= NUMERIC;
                    LED_Matrix_Clear_All(); 
                    timerState=0;
                    break;
            }
        }        

        __delay_ms(LP);
        timerBlink += LP;
        timerState += LP;
    }
    return;
}