/**
 * @file gyro.cpp
 * @brief Implementation of Gyroscope class
 * @author Eren Gunduz
 * @date 05-08-2024
 */

#include "gyro.hpp"
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

using namespace GyroSensor;

Gyroscope::Gyroscope() : fileDescriptor(-1) {}

Gyroscope::~Gyroscope() {
    if (fileDescriptor >= 0) {
        close(fileDescriptor);
    }
}

Gyroscope::ReturnValues Gyroscope::initI2C() 
{
    // Open I2C connection
    if ((fileDescriptor = open(I2C_BUS, O_RDWR)) < 0)
    {
        std::cerr << "Failed to open I2C!" << std::endl;
        return I2C_OPEN_ERR;     
    }
    return SUCCESS;
}

Gyroscope::ReturnValues Gyroscope::readITG3200() 
{
    uint8_t buffer[6];
    buffer[0] = ITG3200_REG_GYRO_XOUT_H;

    // Set the I2C address for ITG3200
    if (ioctl(fileDescriptor, I2C_SLAVE, ITG3200_ADDR) < 0) 
    {
        std::cerr << "Failed to set I2C address for ITG3200." << std::endl;
        return I2C_ADDR_SET_ERR;
    }
    else if (write(fileDescriptor, buffer, 1) != 1) 
    {
        std::cerr << "Failed to write to ITG3200 for gyro read." << std::endl;
        return FAIL_TO_WR_GYRO;
    }
    else if (read(fileDescriptor, buffer, 6) != 6) 
    {
        std::cerr << "Failed to read gyro data from ITG3200." << std::endl;
        return FAIL_TO_RD_GYRO;
    }
    else
    {
        int16_t gyroX = (buffer[0] << 8 | buffer[1]);
        int16_t gyroY = (buffer[2] << 8 | buffer[3]);
        int16_t gyroZ = (buffer[4] << 8 | buffer[5]);
        cout << "Gyro (x, y, z): " << gyroX << ", " << gyroY << ", " << gyroZ << endl;
    }
    return SUCCESS;
}

Gyroscope::ReturnValues Gyroscope::readITG3205() 
{
    uint8_t buffer[6];
    buffer[0] = ITG3200_REG_GYRO_XOUT_H; // ITG3205 uses the same register addresses as ITG3200

    // Set the I2C address for ITG3205
    if (ioctl(fileDescriptor, I2C_SLAVE, ITG3205_ADDR) < 0) 
    {
        std::cerr << "Failed to set I2C address for ITG3205." << std::endl;
        return I2C_ADDR_SET_ERR;
    }
    else if (write(fileDescriptor, buffer, 1) != 1) 
    {
        std::cerr << "Failed to write to ITG3205 for gyro read." << std::endl;
        return FAIL_TO_WR_GYRO;
    }
    else if (read(fileDescriptor, buffer, 6) != 6) 
    {
        std::cerr << "Failed to read gyro data from ITG3205." << std::endl;
        return FAIL_TO_RD_GYRO;
    }
    else
    {
        int16_t gyroX = (buffer[0] << 8 | buffer[1]);
        int16_t gyroY = (buffer[2] << 8 | buffer[3]);
        int16_t gyroZ = (buffer[4] << 8 | buffer[5]);
        std::cout << "ITG3205 Gyro (x, y, z): " << gyroX << ", " << gyroY << ", " << gyroZ << std::endl;
    }
    return SUCCESS;
}