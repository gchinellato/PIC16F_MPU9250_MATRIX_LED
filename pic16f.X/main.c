/*
 * File:   main.c
 * Author: gchinellato
 *
 * Created on April 24, 2017, 10:59 PM
 */

#include <xc.h>

#pragma config FOSC = HS        // Oscillator Selection bits (XT oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

#define _XTAL_FREQ 20000000

#define SLEEP_1S 1000
#define SLEEP_500mS 500
#define SLEEP_200mS 200
#define SLEEP_100mS 100

#define LED0 PORTDbits.RD7
#define SPI_SCK
#define SPI_SDI
#define SPI_SDO
#define SPI_SS PORTDbits.RD0


// MAX7219 SPI LED Driver
#define MATRIX_LED_ROW 8
#define MATRIX_LED_COL 8
 
enum MAX7219_ADDR {
    NO_OP,
    DIGIT_0,
    DIGIT_1,
    DIGIT_2,
    DIGIT_3,
    DIGIT_4,
    DIGIT_5,
    DIGIT_6,
    DIGIT_7,
    DECODE_MODE,
    INTENSITY = 0x0A,
    SCAN_LIMIT,
    SHUTDOWN,
    DISPLAY_TEST = 0x0F,    
};

/*
 * MSSP - MASTER SYNCHRONOUS SERIAL PORT - pg.71 datasheet
 * SSPCON: MSSP Control Register
 * SSPSTAT: MSSP Status Register
 * SSPBUF: Serial Receive/Transmit Buffer Register
 * SSPSR: MSSP Shift Register
 */

void SPI_Write(unsigned char addr, unsigned char data)
{
    // enable the SS SPI pin
    SPI_SS = 0x00;
    
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
    SPI_SS = 0x01;
}

unsigned char SPI_Read(unsigned char addr)
{
    // Activate the SS SPI Select pin
    SPI_SS = 0;
    
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
    SPI_SS = 1;
    
    return(SSPBUF);
}

void initLEDMatrix()
{ 
    char i;
    for(i=1; i <= MATRIX_LED_ROW; i++)
    {
        SPI_Write(i, 0);
    }
}

void main(void) 
{    
    // Configure PORTs
    TRISDbits.TRISD7 = 0x00; // LED output
    TRISDbits.TRISD0 = 0x00; // SPI SS output
    TRISCbits.TRISC3 = 0x00; // SPI SCK output
    TRISCbits.TRISC5 = 0x00; // SPI SDO output
    TRISCbits.TRISC4 = 0x01; // SPI SDI input
    
    //Initial value
    LED0 = 0x00;
    SPI_SS = 0x01; // SS disabled

    // Configure SPI port
    SSPCONbits.SSPM = 0x0; // SPI Master mode, clock = Fosc/4    
    SSPCONbits.SSPEN = 0x1; // Enables serial port   
    SSPCONbits.CKP = 0x0; // Idle state for clock is a low level

    SSPSTATbits.CKE = 0x1; // Transmit occurs on transition from active to Idle clock state
    SSPSTATbits.SMP = 0x0; // Input data sampled at end of data output time    

    //Configure MAX7219
    SPI_Write(DISPLAY_TEST, 0x01); // test mode on
    __delay_ms(SLEEP_1S); 
    SPI_Write(DISPLAY_TEST, 0x00); // test mode off
    SPI_Write(INTENSITY, 0x00); // use lowest intensity
    SPI_Write(SCAN_LIMIT, 0x07); // scan all digits
    SPI_Write(DECODE_MODE, 0x00); //disable BCD mode
    SPI_Write(SHUTDOWN, 0x01); // turn on chip
 
    while(1)
    {
        initLEDMatrix();
        LED0=0x01;
        __delay_ms(SLEEP_1S); 
        
        char i,j;
        for(i=1; i <= MATRIX_LED_ROW; i++)
        {
            for(j=0; j <= MATRIX_LED_COL; j++)
            {
                SPI_Write(i, (1 << j));
                __delay_ms(SLEEP_100mS);  
            }
        }       
        
        LED0=0x00;
        __delay_ms(SLEEP_1S);  
    }
    return;
}
