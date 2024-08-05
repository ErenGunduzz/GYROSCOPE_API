#include <gyro.hpp>
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

ReturnValues initI2C() 
{
    // Open I2C connection
    if ((file = open(I2C_BUS, O_RDWR)) < 0)
    {
        cerr << "I2C not opened!" << endl;
        return_value = I2C_OPEN_ERR;     
    }

    return_value = SUCCESS;
    return return_value;
}

ReturnValues readITG3200(int file) {
    uint8_t buffer[6];
    // Read gyro data from ITG3200
    buffer[0] = ITG3200_REG_GYRO_XOUT_H;

    // set the I2C address for ITG3200
    if (ioctl(file, I2C_SLAVE, ITG3200_ADDR) < 0) {
        cerr << "Failed to set I2C address for ITG3200." << endl;
        return_value = I2C_ADDR_SET_ERR;
    }
    else if (write(file, buffer, 1) != 1) {
        cerr << "Failed to write to ITG3200 for gyro read." << endl;
        return_value = FAIL_TO_WR_GYRO;
    }
    else if (read(file, buffer, 6) != 6) {
        cerr << "Failed to read gyro data from ITG3200." << endl;
        return_value = FAIL_TO_RD_GYRO;
    }
    else 
    {
        int16_t gyro_x = (buffer[0] << 8 | buffer[1]);
        int16_t gyro_y = (buffer[2] << 8 | buffer[3]);
        int16_t gyro_z = (buffer[4] << 8 | buffer[5]);
        cout << "Gyro (x, y, z): " << gyro_x << ", " << gyro_y << ", " << gyro_z << endl;
        return_value = SUCCESS;
    }   
    return return_value; 
}

ReturnValues readITG3205(int file) {
    uint8_t buffer[6];
    // read gyro data from ITG3205
    buffer[0] = ITG3200_REG_GYRO_XOUT_H; // ITG3205 uses the same register addresses as ITG3200


     // set the I2C address for ITG3205
    if (ioctl(file, I2C_SLAVE, ITG3205_ADDR) < 0) {
        cerr << "Failed to set I2C address for ITG3205." << endl;
        return_value = I2C_ADDR_SET_ERR;
    }
    else if (write(file, buffer, 1) != 1) {
        cerr << "Failed to write ITG3205 for gyro read." << endl;
        return_value = FAIL_TO_WR_GYRO;
    }
    else if (read(file, buffer, 6) != 6) {
        cerr << "Failed to read gyro data from ITG3205." << endl;
        return_value = FAIL_TO_RD_GYRO;
    }
    else 
    {
        int16_t gyro_x = (buffer[0] << 8 | buffer[1]);
        int16_t gyro_y = (buffer[2] << 8 | buffer[3]);
        int16_t gyro_z = (buffer[4] << 8 | buffer[5]);

        cout << "ITG3205 Gyro (x, y, z): " << gyro_x << ", " << gyro_y << ", " << gyro_z << endl;
        return_value = SUCCESS;
    }
    return return_value;
}