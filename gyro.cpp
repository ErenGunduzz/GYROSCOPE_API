/**
 * @file gyro.cpp
 * @brief Implementation of gyroscope functions
 * @author Eren Gunduz
 * @date 05-08-2024
 */

#include "gyro.hpp"
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <stdint.h>
#include <cmath>

using namespace std;

int fileDescriptor; /// Global file descriptor

/**
 * @brief Initialize I2C communication
 * @return ReturnValues Indicates success or failure of I2C initialization
 */
ReturnValues initI2C() 
{
    ReturnValues result = SUCCESS;

    // Open I2C connection
    if ((fileDescriptor = open(I2C_BUS, O_RDWR)) < 0)
    {
        cerr << "Failed to open I2C!" << endl;
        result = I2C_OPEN_ERR;     
    }

    return result;
}

/**
 * @brief Read gyroscope data from ITG3200 sensor
 * @param fileDesc File descriptor for I2C communication
 * @return ReturnValues Indicates success or failure of reading operation
 */
ReturnValues readITG3200(int fileDesc) 
{
    ReturnValues result = SUCCESS;
    uint8_t buffer[6];
    buffer[0] = ITG3200_REG_GYRO_XOUT_H;

    // Set the I2C address for ITG3200
    if (ioctl(fileDesc, I2C_SLAVE, ITG3200_ADDR) < 0) 
    {
        cerr << "Failed to set I2C address for ITG3200." << endl;
        result = I2C_ADDR_SET_ERR;
    }
    else if (write(fileDesc, buffer, 1) != 1) 
    {
        cerr << "Failed to write to ITG3200 for gyro read." << endl;
        result = FAIL_TO_WR_GYRO;
    }
    else if (read(fileDesc, buffer, 6) != 6) 
    {
        cerr << "Failed to read gyro data from ITG3200." << endl;
        result = FAIL_TO_RD_GYRO;
    }
    else
    {
        int16_t gyroX = (buffer[0] << 8 | buffer[1]);
        int16_t gyroY = (buffer[2] << 8 | buffer[3]);
        int16_t gyroZ = (buffer[4] << 8 | buffer[5]);
        cout << "Gyro (x, y, z): " << gyroX << ", " << gyroY << ", " << gyroZ << endl;
    }

    return result;
}

/**
 * @brief Read gyroscope data from ITG3205 sensor
 * @param fileDesc File descriptor for I2C communication
 * @return ReturnValues Indicates success or failure of reading operation
 */
ReturnValues readITG3205(int fileDesc) {
    ReturnValues result = SUCCESS;
    uint8_t buffer[6];
    buffer[0] = ITG3200_REG_GYRO_XOUT_H; // ITG3205 uses the same register addresses as ITG3200

    // Set the I2C address for ITG3205
    if (ioctl(fileDesc, I2C_SLAVE, ITG3205_ADDR) < 0) 
    {
        cerr << "Failed to set I2C address for ITG3205." << endl;
        result = I2C_ADDR_SET_ERR;
    }
    else if (write(fileDesc, buffer, 1) != 1) 
    {
        cerr << "Failed to write to ITG3205 for gyro read." << endl;
        result = FAIL_TO_WR_GYRO;
    }
    else if (read(fileDesc, buffer, 6) != 6) 
    {
        cerr << "Failed to read gyro data from ITG3205." << endl;
        result = FAIL_TO_RD_GYRO;
    }
    else
    {
        int16_t gyroX = (buffer[0] << 8 | buffer[1]);
        int16_t gyroY = (buffer[2] << 8 | buffer[3]);
        int16_t gyroZ = (buffer[4] << 8 | buffer[5]);
        cout << "ITG3205 Gyro (x, y, z): " << gyroX << ", " << gyroY << ", " << gyroZ << endl;
    }

    return result;
}