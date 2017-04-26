/* 
 * File:   led_matrix.h
 * Author: gchinellato
 *
 * Created on April 26, 2017, 1:12 AM
 */

#ifndef LED_MATRIX_H
#define	LED_MATRIX_H

#ifdef	__cplusplus
extern "C" {
#endif

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
    
void initLEDMatrix();

#ifdef	__cplusplus
}
#endif

#endif	/* LED_MATRIX_H */

