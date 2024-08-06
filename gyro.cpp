/**
 * @file gyro.cpp
 * @brief Implementation of GyroOperations class
 * @author Eren Gunduz
 * @date 05-08-2024
 */

#include "gyro.hpp"
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

// File descriptor macros 
#define NO_FILE_FD (-1)
#define FILE_OP_VALUE (0)
#define ITG3200_REG_GYRO_INDEX (0)
#define GYRO_READ_BUFFER_SIZE (6)
#define GYRO_WRITE_BUFFER_SIZE (1)
#define SHIFT_VALUE (8)



using namespace GyroSensor;

GyroOperations::GyroOperations() {
    mFileDescriptor = NO_FILE_FD;
}


GyroOperations::~GyroOperations() {
    if (mFileDescriptor >= FILE_OP_VALUE) {
        close(mFileDescriptor);
    }
}

ReturnValues GyroOperations::initI2C() 
{
    ReturnValues result = SUCCESS;

    mFileDescriptor = open(I2C_BUS, O_RDWR);
    // Open I2C connection
    if (mFileDescriptor < FILE_OP_VALUE)
    {
        cerr << "Failed to open I2C!" << endl;
        result = I2C_OPEN_ERR;     
    }
    return result;
}

ReturnValues GyroOperations::readGyro() 
{
    ReturnValues result = SUCCESS;
    uint8_t buffer[GYRO_READ_BUFFER_SIZE];
    buffer[ITG3200_REG_GYRO_INDEX] = ITG3200_REG_GYRO_XOUT_H; // ITG3205 uses the same register addresses as ITG3200

    // Set the I2C address for ITG3205
    if (ioctl(mFileDescriptor, I2C_SLAVE, SENSOR_ADDR) < FILE_OP_VALUE) 
    {
        cerr << "Failed to set I2C address for ITG3205." << endl;
        result = I2C_ADDR_SET_ERR;
    }
    else if (write(mFileDescriptor, buffer, 1) != GYRO_WRITE_BUFFER_SIZE) 
    {
        cerr << "Failed to write to ITG3205 for gyro read." << endl;
        result = FAIL_TO_WR_GYRO;
    }
    else if (read(mFileDescriptor, buffer, GYRO_READ_BUFFER_SIZE) != GYRO_READ_BUFFER_SIZE) 
    {
        cerr << "Failed to read gyro data from ITG3205." << endl;
        result = FAIL_TO_RD_GYRO;
    }
    else
    {
        int16_t gyroX = (buffer[0] << SHIFT_VALUE | buffer[1]);
        int16_t gyroY = (buffer[2] << SHIFT_VALUE | buffer[3]);
        int16_t gyroZ = (buffer[4] << SHIFT_VALUE | buffer[5]);
        cout << "ITG3205 Gyro (x, y, z): " << gyroX << ", " << gyroY << ", " << gyroZ << endl;
        result = SUCCESS;
    }
    return result;
}