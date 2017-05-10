/* 
 * File:   spi.h
 * Author: Guilherme Chinellato
 *         Felipe Leal
 *         William Bispo 
 */

#ifndef SPI_H
#define	SPI_H

#ifdef	__cplusplus
extern "C" {
#endif

//the SPI_SS_X should be disabled in SPI_Init
//#define SPI_SS_0 PORTDbits.RD0
//#define SPI_SS_1 PORTDbits.RD1
#define SPI_SS_0 PORTDbits.RD1
#define SPI_SS_1 PORTDbits.RD0
    
typedef enum {
    SS_0,
    SS_1
}eSlaveSelect;
    
void SPI_Init(void);
void SPI_SS(eSlaveSelect ss);
unsigned char SPI_Read_Write(eSlaveSelect ss, unsigned char addr, unsigned char data);

#ifdef	__cplusplus
}
#endif

#endif	/* SPI_H */

