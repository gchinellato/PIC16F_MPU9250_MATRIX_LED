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

//the SPI_SS_X should be disabled in SPI_Init
#define SPI_SS_0 PORTDbits.RD0
#define SPI_SS_1 PORTDbits.RD1
    
typedef enum {
    SS_0,
    SS_1
}SlaveSelect;
    
void SPI_Init(void);
void SPI_SS(SlaveSelect ss);
unsigned char SPI_Read_Write(SlaveSelect ss, unsigned char addr, unsigned char data);

#ifdef	__cplusplus
}
#endif

#endif	/* SPI_H */

