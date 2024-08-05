/**
 * @file gyro.cpp
 * @brief Implementation of gyroscope functions
 * @author Eren Gunduz
 * @date 05-08-2024
 */

#ifndef GYRO_HPP
#define GYRO_HPP

/**
 * @brief I2C address of ITG3200 sensor
 */
#define ITG3200_ADDR 0x68

/**
 * @brief I2C address of ITG3205 sensor
 */
#define ITG3205_ADDR 0x68

/**
 * @brief I2C bus device file
 */
#define I2C_BUS "/dev/i2c-1"

/**
 * @brief Register address for gyro X-axis high byte output
 */
#define ITG3200_REG_GYRO_XOUT_H 0x1D

/**
 * @brief Enumeration of return values for function calls
 */
enum ReturnValues {
    SUCCESS,          /**< Operation completed successfully */
    I2C_OPEN_ERR,     /**< Error opening I2C bus */
    I2C_ADDR_SET_ERR, /**< Error setting I2C slave address */
    FAIL_TO_WR_GYRO,  /**< Error writing to gyroscope */
    FAIL_TO_RD_GYRO   /**< Error reading from gyroscope */
};

/**
 * @brief Initialize I2C communication
 * @return ReturnValues Indicates success or failure of I2C initialization
 */
ReturnValues initI2C();

/**
 * @brief Read gyroscope data from ITG3200 sensor
 * @param fileDesc File descriptor for I2C communication
 * @return ReturnValues Indicates success or failure of reading operation
 */
ReturnValues readITG3200(int fileDesc);

/**
 * @brief Read gyroscope data from ITG3205 sensor
 * @param fileDesc File descriptor for I2C communication
 * @return ReturnValues Indicates success or failure of reading operation
 */
ReturnValues readITG3205(int fileDesc);

extern int fileDescriptor; /**< Global file descriptor for I2C communication */

#endif // GYRO_HPP