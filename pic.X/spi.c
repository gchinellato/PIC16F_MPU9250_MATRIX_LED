/* 
 * File:   spi.c
 * Author: gchinellato
 *
 * Created on April 26, 2017, 1:11 AM
 */

#include <xc.h>
#include "spi.h"

void SPI_Init(void) {
    TRISDbits.TRISD0 = 0x00; // SPI_SS_0 output
    TRISDbits.TRISD1 = 0x00; // SPI_SS_1 output
    TRISCbits.TRISC3 = 0x00; // SPI SCK output
    TRISCbits.TRISC5 = 0x00; // SPI SDO output
    TRISCbits.TRISC4 = 0x01; // SPI SDI input
    
    // Configure SPI port
    SSPCONbits.SSPM = 0x2; // SPI Master mode, clock = Fosc/64 --> 0.3125MHz
    SSPCONbits.SSPEN = 0x1; // Enables serial port   
    SSPCONbits.CKP = 0x0; // Idle state for clock is a low level
    SSPSTATbits.CKE = 0x1; // Transmit occurs on transition from active to Idle clock state
    SSPSTATbits.SMP = 0x0; // Input data sampled at end of data output time  
    
    //slave select disabled
    SPI_SS_0 = 0x1;
    SPI_SS_1 = 0x1;
}

/*
 * MSSP - MASTER SYNCHRONOUS SERIAL PORT - pg.71 datasheet
 * SSPCON: MSSP Control Register
 * SSPSTAT: MSSP Status Register
 * SSPBUF: Serial Receive/Transmit Buffer Register
 * SSPSR: MSSP Shift Register
 */
void SPI_SS(SlaveSelect ss)
{
    //toogling bit 0
    switch(ss){
        case SS_0:
            SPI_SS_0 ^= (1 << 0);  
            break;
        case SS_1:
            SPI_SS_1 ^= (1 << 0);  
            break;
        default:
            break;
    }
}

unsigned char SPI_Read_Write(SlaveSelect ss, unsigned char addr, unsigned char data)
{
    // enable the SS SPI pin
    SPI_SS(ss);
    
    // Start Register Address transmission
    SSPBUF = addr;
    
     /* Wait for Data Transmit/Receipt complete
     * 
     * BF: Buffer Full Status bit (Receive mode only)
     *  1 = Receive complete, SSPBUF is full
     *  0 = Receive not complete, SSPBUF is empty
     */
    while(!SSPSTATbits.BF);            

    // Start Data transmission
    SSPBUF = data;
    
    // Wait for Data Transmit/Receipt complete
    while(!SSPSTATbits.BF);

    // disable the SS SPI pin
    SPI_SS(ss);
    
    return SSPBUF;
}