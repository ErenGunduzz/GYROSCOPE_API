/**
 * @file gyro.cpp
 * @brief Implementation of GyroOperations class
 * @author Eren Gunduz
 * @date 05-08-2024
 */

#include "gyroscope.hpp"
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

using namespace std;

namespace OBU {

    uint8_t const GyroOperations::SENSOR_ADDR = 0x68;
    string const GyroOperations::I2C_BUS = "/dev/i2c-1";
    uint8_t const GyroOperations::SENSOR_REG_GYRO_XOUT_H = 0x43; //0x1D for ITG3205
    uint8_t const GyroOperations::SENSOR_REG_POWER_CTL = 0x6B; // 0x6B is the power management register
    // const uint8_t const GyroOperations::SENSOR_REG_DATAX0 = 0x32;
    uint8_t const GyroOperations::SENSOR_MEASURE_MODE = 0x00;
    int32_t const GyroOperations::NO_FILE_FD = -1;
    int32_t const GyroOperations::FILE_OP_VALUE = 0;
    size_t const GyroOperations::WR_REG_BUFFER = 2;
    int32_t const GyroOperations::SHIFT_VALUE = 8;
    size_t const GyroOperations::BUFFER_SIZE = 6;
    size_t const GyroOperations::WR_REG_BUFFER_SIZE = 1;
    int32_t const GyroOperations::X_AXIS_START_VALUE = 0;
    int32_t const GyroOperations::Y_AXIS_START_VALUE = 0;
    int32_t const GyroOperations::Z_AXIS_START_VALUE = 0;
    int32_t const GyroOperations::BUFFER_START_VALUE = 0;

    GyroOperations::GyroOperations() {
        this->device = I2C_BUS;
        mFileDescriptor = NO_FILE_FD;
    }

    GyroOperations::~GyroOperations() {
        if (mFileDescriptor >= FILE_OP_VALUE) {
            close(mFileDescriptor);
        }
    }

    /**
     * @brief Getter for device.
     * @return The value of device.
     */
    string GyroOperations::getDeviceName() const{
        return (device);
    }

    /**
     * @brief Setter for device.
     * @param value The value to set for device.
     */
    void GyroOperations::setDeviceName(std::string device) {
        this->device = move(device);
    }

    /**
     * @brief Getter for mFileDescriptor.
     * @return The value of mFileDescriptor.
     */
    int32_t GyroOperations::getFileDescriptor() const {
        return (mFileDescriptor);
    }

    /**
     * @brief Setter for device.
     * @param value The value to set for device.
     */
    void GyroOperations::setFileDescriptor(int32_t fileDescriptor) {
        mFileDescriptor = fileDescriptor;
    }

    /**
     * @brief Getter for scaleFactor.
     * @return The value of scaleFactor.
     */
    double GyroOperations::getScaleFactor() const {
        return (scaleFactor);
    }

    /**
     * @brief Setter for scaleFactor.
     * @param value The value to set for scaleFactor.
     */
    void GyroOperations::setScaleFactor(double factor) {
        scaleFactor = factor;
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
            if (writeRegister(SENSOR_REG_POWER_CTL, SENSOR_MEASURE_MODE) != GyroError::SUCCESS) { // SENSOR_REG_POWER_CTL is the power management register
                cerr << "Failed to wake up sensor." << endl;
                result = GyroError::SENSOR_WAKE_FAIL;
            }
        }
        return (result);
    }

    GyroError GyroOperations::readGyro(double& x, double& y, double& z) {
        uint8_t buffer[BUFFER_SIZE] = {BUFFER_START_VALUE};

        GyroError result = readRegisters(SENSOR_REG_GYRO_XOUT_H, buffer, BUFFER_SIZE);
        if (result == GyroError::SUCCESS) {
            x = ((buffer[0] << SHIFT_VALUE) | buffer[1]) / scaleFactor;
            y = ((buffer[2] << SHIFT_VALUE) | buffer[3]) / scaleFactor;
            z = ((buffer[4] << SHIFT_VALUE) | buffer[5]) / scaleFactor;
        }
        return (result);
    }

    GyroError GyroOperations::writeRegister(uint8_t reg, uint8_t value) {
        uint8_t buffer[WR_REG_BUFFER] = {reg, value};
        GyroError result = GyroError::SUCCESS;

        if (write(mFileDescriptor, buffer, WR_REG_BUFFER) != WR_REG_BUFFER) {
            result = GyroError::I2C_WRITE_ERR;
        }
        return (result);
    }

    GyroError GyroOperations::readRegisters(uint8_t reg, uint8_t* buffer, size_t length) {
        GyroError result = GyroError::SUCCESS;
        if (write(mFileDescriptor, &reg, WR_REG_BUFFER_SIZE) != WR_REG_BUFFER_SIZE) {
            result = GyroError::I2C_WRITE_ERR;
        }
        if (read(mFileDescriptor, buffer, length) != static_cast<int>(length)) {
            result = GyroError::I2C_READ_ERR;
        }
        return (GyroError::SUCCESS);
    }

    GyroscopeData GyroOperations::getGyroData() {
        GyroscopeData data = {X_AXIS_START_VALUE, Y_AXIS_START_VALUE, Z_AXIS_START_VALUE};

        if (readGyro(data.x, data.y, data.z) != GyroError::SUCCESS) {
            cerr << "Failed to read gyroscope data!" << endl;
        }

        return (data);
    }

} // namespace OBU
