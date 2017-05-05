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

void main(void) 
{  
    //Init UART
    USART_Init();
    
    //Init SPI
    SPI_Init();
    
    //Init LED Matrix and MAX7219
    LED_Matrix_Init();
 
    //IMU Init
    IMU_Init();    
    printf("IMU WHO AM I:%x\r\n", IMU_WhoAmI()); 
    
    LED_Matrix_Clear();
   
     while(1)
    {   
        char i,j;
        /*for(i=1; i <= LED_MATRIX_COL; i++)
        {
            for(j=0; j <= LED_MATRIX_ROW; j++)
            {
                LED_Matrix_Update(i,j);
                __delay_ms(10);  
            }
        }*/

        accel accel;
        IMU_Accel_Read(&accel);
        gyro gyro;
        IMU_Gyro_Read(&gyro);        
                
        //printf("\naccel: %f %f %f [g]\r\n", accel.x, accel.y, accel.z);        
        //printf("gyro : %08f %08f %08f [deg/s]\r\n", gyro.x, gyro.y, gyro.z);
        
        float roll;
        float pitch;
        float yaw;
        
        roll = atan2(accel.y, accel.z);
        pitch = atan2(accel.x, accel.z); 
        //printf("roll: %f pitch: %f\r\n", roll*RAD_TO_DEG, pitch*RAD_TO_DEG);  
        
        float factor_x, factor_j;
        factor_x = ((roll*RAD_TO_DEG)-(-90.0))/(90.0-(-90.0));
        factor_j = ((pitch*RAD_TO_DEG)-(-90.0))/(90.0-(-90.0));
        i=factor_x*(8-1)+1; 
        j=factor_j*(7-0)+0; 
        
        printf("i:%d j:%d\r\n", i, j);  
        
        char x,y;
        for(x=1; x <= LED_MATRIX_COL; x++)
        {
            LED_Matrix_Update(x,8);   
        }
        
        LED_Matrix_Update(i,j);
         
        __delay_ms(10);

    }
    return;
}