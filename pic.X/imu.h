/* 
 * File:   imu.h
 * Author: gchinellato
 *
 * Created on April 26, 2017, 1:12 AM
 */

#ifndef IMU_H
#define	IMU_H

#ifdef	__cplusplus
extern "C" {
#endif

#define G 9.807
#define DEG_TO_RAD 3.1415926/180.0    

//IMU MPU9250 register for Gyro and Accel
#define SELF_TEST_X_GYRO    0x00
#define SELF_TEST_Y_GYRO    0x01
#define SELF_TEST_Z_GYRO    0x02
#define SELF_TEST_X_ACCEL   0x0D
#define SELF_TEST_Y_ACCEL   0x0E
#define SELF_TEST_Z_ACCEL   0x0F
    
#define XG_OFFSET_H         0x13
#define XG_OFFSET_L         0x14
#define YG_OFFSET_H         0x15
#define YG_OFFSET_L         0x16
#define ZG_OFFSET_H         0x17
#define ZG_OFFSET_L         0x18
    
#define SMPLRT_DIV          0x19  
    
#define I2C_MST_CTRL        0x24
    
#define CONFIG              0x1A
#define GYRO_DLPF_184       0x01
#define GYRO_DLPF_92        0x02
#define GYRO_DLPF_41        0x03
#define GYRO_DLPF_20        0x04
#define GYRO_DLPF_10        0x05
#define GYRO_DLPF_5         0x06
    
#define GYRO_CONFIG         0x1B
#define GYRO_FS_SEL_250DPS  0x00
#define GYRO_FS_SEL_500DPS  0x08
#define GYRO_FS_SEL_1000DPS 0x10
#define GYRO_FS_SEL_2000DPS 0x18
    
#define ACCEL_CONFIG        0x1C
#define ACCEL_FS_SEL_2G     0x00
#define ACCEL_FS_SEL_4G     0x08
#define ACCEL_FS_SEL_8G     0x10
#define ACCEL_FS_SEL_16G    0x18
    
#define ACCEL_CONFIG_2      0x1D
#define ACCEL_DLPF_184      0x01
#define ACCEL_DLPF_92       0x02
#define ACCEL_DLPF_41       0x03
#define ACCEL_DLPF_20       0x04
#define ACCEL_DLPF_10       0x05
#define ACCEL_DLPF_5        0x06
    
#define ACCEL_XOUT_H        0x3B
#define ACCEL_XOUT_L        0x3C
#define ACCEL_YOUT_H        0x3D
#define ACCEL_YOUT_L        0x3E
#define ACCEL_ZOUT_H        0x3F
#define ACCEL_ZOUT_L        0x40
    
#define TEMP_OUT_H          0x41
#define TEMP_OUT_L          0x42
    
#define GYRO_XOUT_H         0x43
#define GYRO_XOUT_L         0x44
#define GYRO_YOUT_H         0x45
#define GYRO_YOUT_L         0x46
#define GYRO_ZOUT_H         0x47
#define GYRO_ZOUT_L         0x48
    
#define SIGNAL_PATH_RESET   0x68
#define RESET_SIGNAL        0x10
    
#define MOT_DETECT_CTRL     0x69
    
#define USER_CTRL           0x6A
#define DISABLE_I2C         0x10
    
#define PWR_MGMT_1          0x6B
#define PWR_RESET           0x80
#define CLOCK_SEL_PLL       0x01
    
#define PWR_MGMT_2          0x6C
#define SEN_ENABLE          0x00
    
#define WHO_AM_I            0x75
#define DEVICE_ID           0x71

#ifdef	__cplusplus
}
#endif

#endif	/* IMU_H */

