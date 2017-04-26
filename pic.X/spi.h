/* 
 * File:   spi.h
 * Author: gchinellato
 *
 * Created on April 26, 2017, 1:11 AM
 */

#ifndef SPI_H
#define	SPI_H

#ifdef	__cplusplus
extern "C" {
#endif

#define SPI_SS_1 PORTDbits.RD0
    
void SPI_Init(void);
void SPI_Write(unsigned char addr, unsigned char data);
unsigned char SPI_Read(unsigned char addr);

#ifdef	__cplusplus
}
#endif

#endif	/* SPI_H */

