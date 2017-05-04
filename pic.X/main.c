/*
 * File:   main.c
 * Author: gchinellato
 *
 * Created on April 24, 2017, 10:59 PM
 */

#include <xc.h>
#include <stdio.h>
#include "spi.h"
#include "serial.h"
#include "led_matrix.h"
#include "imu.h"

#pragma config FOSC = HS        // Oscillator Selection bits (XT oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

#define _XTAL_FREQ 20000000
#define LED0 PORTDbits.RD7

int getSignedNumber(int number)
{
    if(number > 32767)
    {
        number -= 65536;
    }
    return number;
}

void main(void) 
{    
    // Configure PORTs
    TRISDbits.TRISD7 = 0x00; // LED output
    
    //Initial value
    LED0 = 0x00;
    
    //Init UART
    USART_Init();
    
    //Init SPI
    SPI_Init();
    
    //Configure MAX7219
    SPI_Read_Write(SS_0, DISPLAY_TEST, 0x01); // test mode on
    __delay_ms(500); 
    SPI_Read_Write(SS_0, DISPLAY_TEST, 0x00); // test mode off
    SPI_Read_Write(SS_0, INTENSITY, 0x00); // use lowest intensity
    SPI_Read_Write(SS_0, SCAN_LIMIT, 0x07); // scan all digits
    SPI_Read_Write(SS_0, DECODE_MODE, 0x00); //disable BCD mode
    SPI_Read_Write(SS_0, SHUTDOWN, 0x01); // turn on chip
 
    //Configure IMU
    //reset MPU9250
    SPI_Read_Write(SS_1, PWR_MGMT_1, PWR_RESET);    
    __delay_ms(100); 
    //select clock source to gyro
    SPI_Read_Write(SS_1, PWR_MGMT_1, CLOCK_SEL_PLL); 
    //enable accel and gyro
    SPI_Read_Write(SS_1, PWR_MGMT_2, SEN_ENABLE);
    //disable I2C protocol
    SPI_Read_Write(SS_1, USER_CTRL, DISABLE_I2C); 
    
    //setup accel and gyro ranges
    SPI_Read_Write(SS_1, ACCEL_CONFIG, ACCEL_FS_SEL_2G);   
    SPI_Read_Write(SS_1, GYRO_CONFIG, GYRO_FS_SEL_250DPS);
    
    float accelScale = 2.0/32768.0; 
    float gyroScale = 250.0/32768.0 * DEG_TO_RAD;
    
    //check WHO AM I
    printf("gc >>> IMU WHO AM I:%x\r\n", SPI_Read_Write(SS_1, WHO_AM_I|READ_FLAG, 0x0));
    
    printf("gc >>> IMU initialization done\r\n\n\n");
   
     while(1)
    {
        LED0=0x01;
        initLEDMatrix();
        
        //LED MATRIX 
        char i,j;
        for(i=1; i <= MATRIX_LED_ROW; i++)
        {
            for(j=0; j <= MATRIX_LED_COL; j++)
            {
                SPI_Read_Write(SS_0, i, (1 << j));
                //printf("gc >>> i:%d, j:%d\r\n", i, j);
                __delay_ms(10);  
            }
        }

        printf("IMU WHO AM I:%x\r\n", SPI_Read_Write(SS_1, WHO_AM_I|READ_FLAG, 0x0));
        //READ ACCEL 
        int accel_x;
        int accel_y;
        int accel_z;

        accel_x = ((int)SPI_Read_Write(SS_1, ACCEL_XOUT_H|READ_FLAG, 0x0) << 8) | SPI_Read_Write(SS_1, ACCEL_XOUT_L|READ_FLAG, 0x0);
        //accel_x = getSignedNumber(accel_x);
        accel_y = ((int)SPI_Read_Write(SS_1, ACCEL_YOUT_H|READ_FLAG, 0x0) << 8) | SPI_Read_Write(SS_1, ACCEL_YOUT_L|READ_FLAG, 0x0);
        //accel_y = getSignedNumber(accel_y);
        accel_z = ((int)SPI_Read_Write(SS_1, ACCEL_ZOUT_H|READ_FLAG, 0x0) << 8) | SPI_Read_Write(SS_1, ACCEL_ZOUT_L|READ_FLAG, 0x0);
        //accel_z = getSignedNumber(accel_z);
        
        printf("ACCEL_XOUT:%d %d\r\n", SPI_Read_Write(SS_1, ACCEL_XOUT_H|READ_FLAG, 0x0), SPI_Read_Write(SS_1, ACCEL_XOUT_L|READ_FLAG, 0x0));
        printf("ACCEL_YOUT:%d %d\r\n", SPI_Read_Write(SS_1, ACCEL_YOUT_H|READ_FLAG, 0x0), SPI_Read_Write(SS_1, ACCEL_YOUT_L|READ_FLAG, 0x0));
        printf("ACCEL_ZOUT:%d %d\r\n", SPI_Read_Write(SS_1, ACCEL_ZOUT_H|READ_FLAG, 0x0), SPI_Read_Write(SS_1, ACCEL_ZOUT_L|READ_FLAG, 0x0));
                
        printf("\n[RAW](x y z): %d %d %d\r\n", accel_x, accel_y, accel_z);        
        printf("[ G ](x y z): %f %f %f\r\n\n", accel_x*accelScale, accel_y*accelScale, accel_z*accelScale); 
       
        LED0=0x00;
    }
    return;
}