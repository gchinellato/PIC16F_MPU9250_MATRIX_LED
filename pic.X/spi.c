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
    SSPCONbits.SSPM = 0x1; // SPI Master mode, clock = Fosc/16 --> 1.25MHz
    SSPCONbits.SSPEN = 0x1; // Enables serial port  
    
    /*         CKP | CKE
     * MODE 0:  0  |  0
     * MODE 1:  0  |  1
     * MODE 2:  1  |  0
     * MODE 3:  1  |  1
     */    
    SSPCONbits.CKP = 0x1; // Clock Polarity Select (idle state)
    SSPSTATbits.CKE = 0x0; // SPI Clock Select bit (Transmit occurs on transition from idle to active)
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