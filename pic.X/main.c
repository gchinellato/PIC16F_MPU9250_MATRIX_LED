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
    SPI_SS_1 = 0x01; // SS disabled
    
    //Init UART
    USART_Init();
    
    //Init SPI
    SPI_Init();
    
    //Configure MAX7219
    SPI_Write(DISPLAY_TEST, 0x01); // test mode on
    __delay_ms(500); 
    SPI_Write(DISPLAY_TEST, 0x00); // test mode off
    SPI_Write(INTENSITY, 0x00); // use lowest intensity
    SPI_Write(SCAN_LIMIT, 0x07); // scan all digits
    SPI_Write(DECODE_MODE, 0x00); //disable BCD mode
    SPI_Write(SHUTDOWN, 0x01); // turn on chip
 
     while(1)
    {
        initLEDMatrix();
        LED0=0x01;
        
        char i,j;
        for(i=1; i <= MATRIX_LED_ROW; i++)
        {
            for(j=0; j <= MATRIX_LED_COL; j++)
            {
                SPI_Write(i, (1 << j));
                printf("gc >>> i:%d, j:%d\r\n", i, j);
                __delay_ms(20);  
            }
        }
       
        LED0=0x00;
        __delay_ms(20);  
    }
    return;
}