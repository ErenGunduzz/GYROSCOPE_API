/**
 * @file gyro.cpp
 * @brief Implementation of GyroOperations class
 * @author Eren Gunduz
 * @date 05-08-2024
 */

#include "gyroscope.hpp"
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

using namespace std;

namespace OBU {

const uint8_t GyroOperations::SENSOR_ADDR = 0x68;
const string GyroOperations::I2C_BUS = "/dev/i2c-1";
const uint8_t GyroOperations::SENSOR_REG_GYRO_XOUT_H = 0x1D;
const int GyroOperations::NO_FILE_FD = -1;
const int GyroOperations::FILE_OP_VALUE = 0;
const int GyroOperations::GYRO_WR_REG = 2;
const int GyroOperations::SHIFT_VALUE = 8;
const size_t GyroOperations::BUFFER_SIZE = 6;
const size_t GyroOperations::WR_REG_BUFFER_SIZE = 1;
const int GyroOperations::X_AXIS_START_VALUE = 0;
const int GyroOperations::Y_AXIS_START_VALUE = 0;
const int GyroOperations::Z_AXIS_START_VALUE = 0;

GyroOperations::GyroOperations() {
    this->device = I2C_BUS;
    mFileDescriptor = NO_FILE_FD;
}

GyroOperations::~GyroOperations() {
    if (mFileDescriptor >= FILE_OP_VALUE) {
        close(mFileDescriptor);
    }
}

GyroError GyroOperations::initialize() {
    GyroError result = GyroError::SUCCESS;

    mFileDescriptor = open(device.c_str(), O_RDWR);
    if (mFileDescriptor < FILE_OP_VALUE) {
        cerr << "Failed to open I2C bus!" << endl;
        result = GyroError::I2C_OPEN_ERR;
    } else if (ioctl(mFileDescriptor, I2C_SLAVE, SENSOR_ADDR) < FILE_OP_VALUE) {
        cerr << "Failed to set I2C address for sensor." << endl;
        result = GyroError::I2C_ADDR_SET_ERR;
    }

    return result;
}

GyroError GyroOperations::readGyro(int16_t& x, int16_t& y, int16_t& z) {
    uint8_t buffer[BUFFER_SIZE];
    buffer[0] = SENSOR_REG_GYRO_XOUT_H;
    GyroError result = GyroError::SUCCESS;

    if (write(mFileDescriptor, buffer, WR_REG_BUFFER_SIZE) != WR_REG_BUFFER_SIZE) {
        cerr << "Failed to write to sensor for gyro read." << endl;
        result = GyroError::FAIL_TO_WR_GYRO;
    } else if (read(mFileDescriptor, buffer, BUFFER_SIZE) != BUFFER_SIZE) {
        cerr << "Failed to read gyro data from sensor." << endl;
        result = GyroError::FAIL_TO_RD_GYRO;
    } else {
        x = static_cast<int16_t>((buffer[0] << SHIFT_VALUE) | buffer[1]);
        y = static_cast<int16_t>((buffer[2] << SHIFT_VALUE) | buffer[3]);
        z = static_cast<int16_t>((buffer[4] << SHIFT_VALUE) | buffer[5]);
    }

    return result;
}

GyroError GyroOperations::writeRegister(uint8_t reg, uint8_t value) {
    uint8_t buffer[GYRO_WR_REG] = { reg, value };
    GyroError result = GyroError::SUCCESS;

    if (write(mFileDescriptor, buffer, GYRO_WR_REG) != GYRO_WR_REG) {
        cerr << "Failed to write to the i2c bus" << endl;
        result = GyroError::FAIL_TO_WR_GYRO;
    }

    return result;
}

GyroError GyroOperations::readRegisters(uint8_t reg, uint8_t* buffer, size_t length) {
    GyroError result = GyroError::SUCCESS;

    if (write(mFileDescriptor, &reg, WR_REG_BUFFER_SIZE) != WR_REG_BUFFER_SIZE) {
        cerr << "Failed to write to the i2c bus" << endl;
        result = GyroError::FAIL_TO_WR_GYRO;
    } else if (read(mFileDescriptor, buffer, length) != static_cast<int>(length)) {
        cerr << "Failed to read from the i2c bus" << endl;
        result = GyroError::FAIL_TO_RD_GYRO;
    }

    return result;
}

GyroscopeData GyroOperations::getGyroData() {
    int16_t x = X_AXIS_START_VALUE, y = Y_AXIS_START_VALUE, z = Z_AXIS_START_VALUE;
    GyroscopeData gyroData;

    if (readGyro(x, y, z) == GyroError::SUCCESS) {
        gyroData.x = x / scaleFactor;
        gyroData.y = y / scaleFactor;
        gyroData.z = z / scaleFactor;
    }

    return gyroData;
}

} // namespace OBU
