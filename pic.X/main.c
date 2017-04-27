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
    __delay_ms(1000);    
    //disable I2C protocol
    SPI_Read_Write(SS_1, USER_CTRL, DISABLE_I2C); 
    //SPI_Write(SS_1, SIGNAL_PATH_RESET, RESET_SIGNAL);     
    //select clock source to gyro
    SPI_Read_Write(SS_1, PWR_MGMT_1, CLOCK_SEL_PLL); 
    //check WHO AM I
    printf("gc >>> IMU WHO AM I:%x\r\n", SPI_Read_Write(SS_1, WHO_AM_I, 0x0));
    //enable accel and gyro
    SPI_Read_Write(SS_1, PWR_MGMT_2, SEN_ENABLE);
    //setup accel and gyro ranges
    SPI_Read_Write(SS_1, ACCEL_CONFIG, ACCEL_FS_SEL_2G);   
    SPI_Read_Write(SS_1, GYRO_CONFIG, GYRO_FS_SEL_250DPS);
    
    float accelScale = G * 2.0/32767.5; 
    float gyroScale = 250.0/32767.5 * DEG_TO_RAD;
    
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
        
        __delay_ms(10);
        printf("gc >>> IMU WHO AM I:%x\r\n", SPI_Read_Write(SS_1, WHO_AM_I, 0x0));
        
        //IMU
        //READ ACCEL  
        printf("gc >>> ACCEL_XOUT_H:%x\r\n", SPI_Read_Write(SS_1, ACCEL_XOUT_H, 0x0));
        printf("gc >>> ACCEL_XOUT_L:%x\r\n", SPI_Read_Write(SS_1, ACCEL_XOUT_L, 0x0));
        printf("gc >>> ACCEL_YOUT_H:%x\r\n", SPI_Read_Write(SS_1, ACCEL_YOUT_H, 0x0));
        printf("gc >>> ACCEL_YOUT_L:%x\r\n", SPI_Read_Write(SS_1, ACCEL_YOUT_L, 0x0));
        printf("gc >>> ACCEL_ZOUT_H:%x\r\n", SPI_Read_Write(SS_1, ACCEL_ZOUT_H, 0x0));
        printf("gc >>> ACCEL_ZOUT_L:%x\r\n", SPI_Read_Write(SS_1, ACCEL_ZOUT_L, 0x0));
       
        LED0=0x00;
        __delay_ms(10);  
    }
    return;
}