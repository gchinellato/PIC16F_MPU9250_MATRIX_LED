/* 
 * File:   spi.c
 * Author: gchinellato
 *
 * Created on April 26, 2017, 1:11 AM
 */

#include <xc.h>
#include "spi.h"

void SPI_Init(void) {
    TRISDbits.TRISD0 = 0x00; // SPI SS output
    TRISCbits.TRISC3 = 0x00; // SPI SCK output
    TRISCbits.TRISC5 = 0x00; // SPI SDO output
    TRISCbits.TRISC4 = 0x01; // SPI SDI input
    
    // Configure SPI port
    SSPCONbits.SSPM = 0x0; // SPI Master mode, clock = Fosc/4    
    SSPCONbits.SSPEN = 0x1; // Enables serial port   
    SSPCONbits.CKP = 0x0; // Idle state for clock is a low level

    SSPSTATbits.CKE = 0x1; // Transmit occurs on transition from active to Idle clock state
    SSPSTATbits.SMP = 0x0; // Input data sampled at end of data output time    
}

/*
 * MSSP - MASTER SYNCHRONOUS SERIAL PORT - pg.71 datasheet
 * SSPCON: MSSP Control Register
 * SSPSTAT: MSSP Status Register
 * SSPBUF: Serial Receive/Transmit Buffer Register
 * SSPSR: MSSP Shift Register
 */

void SPI_Write(unsigned char addr, unsigned char data) {
    // enable the SS SPI pin
    SPI_SS_1 = 0x00;
    
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
    SPI_SS_1 = 0x01;
}

unsigned char SPI_Read(unsigned char addr)
{
    // Activate the SS SPI Select pin
    SPI_SS_1 = 0;
    
    // Wait for Data Transmit/Receipt complete
    while(!SSPSTATbits.BF); 
    
    // Start Address transmission
    SSPBUF = addr;
    
    // Wait for Data Transmit/Receipt complete
    while(!SSPSTATbits.BF);  

    // Send Dummy transmission for reading the data
    SSPBUF = 0x00;
    
    // Wait for Data Transmit/Receipt complete
    while(!SSPSTATbits.BF); 

    // disable the SS SPI pin
    SPI_SS_1 = 1;
    
    return(SSPBUF);
}