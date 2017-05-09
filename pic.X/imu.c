/* 
 * File:   imu.c
 * Author: gchinellato
 *
 * Created on April 26, 2017, 1:12 AM
 */

#include "imu.h"
#include "spi.h"

void IMU_Init(void)
{
    //reset MPU9250
    SPI_Read_Write(SS_1, PWR_MGMT_1, PWR_RESET);    
    __delay_ms(100); 
    //select clock source to gyro
    SPI_Read_Write(SS_1, PWR_MGMT_1, CLOCK_SEL_PLL); 
    __delay_ms(100); 
    //enable accel and gyro
    SPI_Read_Write(SS_1, PWR_MGMT_2, SEN_ENABLE);
    //disable I2C protocol
    SPI_Read_Write(SS_1, USER_CTRL, DISABLE_I2C); 
    
    //setup accel and gyro ranges
    IMU_Accel_SetScale(ACCEL_FS_SEL_2G);
    IMU_Gyro_SetScale(GYRO_FS_SEL_250DPS);
    
    IMU_WhoAmI();
}

void IMU_Accel_SetScale(char scale)
{
    //set accel scale
    SPI_Read_Write(SS_1, ACCEL_CONFIG, scale); 
    
    switch(scale)
    {
        case ACCEL_FS_SEL_2G:
            gAccelScale = 2.0/32768.0; 
            break;
        case ACCEL_FS_SEL_4G:
            gAccelScale = 4.0/32768.0; 
            break;
        case ACCEL_FS_SEL_8G:
            gAccelScale = 8.0/32768.0; 
            break;
        case ACCEL_FS_SEL_16G:
            gAccelScale = 16.0/32768.0; 
            break;
    }   
}

void IMU_Gyro_SetScale(char scale)
{
    //set gyro scale
    SPI_Read_Write(SS_1, GYRO_CONFIG, scale); 
    
    switch(scale)
    {
        case GYRO_FS_SEL_250DPS:
            gGyroScale = 250.0/32768.0; 
            break;
        case GYRO_FS_SEL_500DPS:
            gGyroScale = 500.0/32768.0; 
            break;
        case GYRO_FS_SEL_1000DPS:
            gGyroScale = 1000.0/32768.0; 
            break;
        case GYRO_FS_SEL_2000DPS:
            gGyroScale = 2000.0/32768.0; 
            break;
    }   
}

char IMU_WhoAmI(void)
{
    //get Device ID == 0x71
    return SPI_Read_Write(SS_1, WHO_AM_I|READ_FLAG, 0x0);  
}

void IMU_Accel_Read(accel* accel)
{
    //convert 8-bit register in 16-bit register
    accel->x = ((int)SPI_Read_Write(SS_1, ACCEL_XOUT_H|READ_FLAG, 0x0) << 8) | SPI_Read_Write(SS_1, ACCEL_XOUT_L|READ_FLAG, 0x0);
    accel->y = ((int)SPI_Read_Write(SS_1, ACCEL_YOUT_H|READ_FLAG, 0x0) << 8) | SPI_Read_Write(SS_1, ACCEL_YOUT_L|READ_FLAG, 0x0);
    accel->z = ((int)SPI_Read_Write(SS_1, ACCEL_ZOUT_H|READ_FLAG, 0x0) << 8) | SPI_Read_Write(SS_1, ACCEL_ZOUT_L|READ_FLAG, 0x0);
    
    //convert to force-g
    accel->x *= gAccelScale;
    accel->y *= gAccelScale;
    accel->z *= gAccelScale;
}

void IMU_Gyro_Read(gyro* gyro)
{
     //convert 8-bit register in 16-bit register
    gyro->x = ((int)SPI_Read_Write(SS_1, GYRO_XOUT_H|READ_FLAG, 0x0) << 8) | SPI_Read_Write(SS_1, GYRO_XOUT_L|READ_FLAG, 0x0);
    gyro->y = ((int)SPI_Read_Write(SS_1, GYRO_YOUT_H|READ_FLAG, 0x0) << 8) | SPI_Read_Write(SS_1, GYRO_YOUT_L|READ_FLAG, 0x0);
    gyro->z = ((int)SPI_Read_Write(SS_1, GYRO_ZOUT_H|READ_FLAG, 0x0) << 8) | SPI_Read_Write(SS_1, GYRO_ZOUT_L|READ_FLAG, 0x0);
    
    //convert to deg/s
    gyro->x *= gGyroScale;
    gyro->y *= gGyroScale;
    gyro->z *= gGyroScale;
}