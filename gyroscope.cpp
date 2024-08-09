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
const uint8_t GyroOperations::SENSOR_REG_GYRO_XOUT_H = 0x43; //0x1D for ITG3205
// const uint8_t GyroOperations::SENSOR_REG_POWER_CTL = 0x6B; // 0x6B is the power management register
// const uint8_t GyroOperations::SENSOR_REG_DATAX0 = 0x32;
// const uint8_t GyroOperations::SENSOR_MEASURE_MODE = 0x08;
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
    } else {
        // Wake up the sensor as it starts in sleep mode
        if (writeRegister(0x6B, 0x00) != GyroError::SUCCESS) { // 0x6B is the power management register
            cerr << "Failed to wake up MPU6050." << endl;
            result = GyroError::I2C_WRITE_ERR;
        }
    }

    return result;
}

GyroError GyroOperations::readGyro(double& x, double& y, double& z) {
    uint8_t buffer[BUFFER_SIZE] = {0};

    GyroError result = readRegisters(SENSOR_REG_GYRO_XOUT_H, buffer, BUFFER_SIZE);
    if (result == GyroError::SUCCESS) {
        x = ((buffer[0] << SHIFT_VALUE) | buffer[1]) / scaleFactor;
        y = ((buffer[2] << SHIFT_VALUE) | buffer[3]) / scaleFactor;
        z = ((buffer[4] << SHIFT_VALUE) | buffer[5]) / scaleFactor;
    }

    return result;
}

GyroError GyroOperations::writeRegister(uint8_t reg, uint8_t value) {
    uint8_t buffer[2] = {reg, value};
    GyroError result = GyroError::SUCCESS;

    if (write(mFileDescriptor, buffer, 2) != 2) {
        result = GyroError::I2C_WRITE_ERR;
    }
    return result;
}

GyroError GyroOperations::readRegisters(uint8_t reg, uint8_t* buffer, size_t length) {
    GyroError result = GyroError::SUCCESS;
    if (write(mFileDescriptor, &reg, 1) != 1) {
        result = GyroError::I2C_WRITE_ERR;
    }
    if (read(mFileDescriptor, buffer, length) != static_cast<int>(length)) {
        result = GyroError::I2C_READ_ERR;
    }
    return GyroError::SUCCESS;
}

GyroscopeData GyroOperations::getGyroData() {
    GyroscopeData data = {X_AXIS_START_VALUE, Y_AXIS_START_VALUE, Z_AXIS_START_VALUE};

    if (readGyro(data.x, data.y, data.z) != GyroError::SUCCESS) {
        cerr << "Failed to read gyroscope data!" << endl;
    }

    return data;
}

} // namespace OBU
