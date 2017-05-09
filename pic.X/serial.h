/* 
 * File:   serial.h
 * Author: gchinellato
 *
 * Created on April 26, 2017, 1:12 AM
 */

#ifndef SERIAL_H
#define	SERIAL_H

#ifdef	__cplusplus
extern "C" {
#endif

void USART_Init(void);
void putch(unsigned char chr);

#ifdef	__cplusplus
}
#endif

#endif	/* SERIAL_H */

