#ifndef GYRO_HPP
#define GYRO_HPP

/**
 * @file gyro.hpp
 * @brief Gyroscope API header file
 */

/**
 * @brief A macro that store the address of ITG3200 sensor
 */
// I2C addresses og gyro sensors
#define ITG3200_ADDR 0x68
/**
 * @brief A macro that store the address of ITG3205 sensor
 */
#define ITG3205_ADDR 0x68

/**
 * @brief A macro that store the I2C bus info
 */
// I2C bus
#define I2C_BUS "/dev/i2c-1"

/**
 * @brief A macro that store the address of ITG3200 sensor output register
 */
// Register address for the sensors
#define ITG3200_REG_GYRO_XOUT_H 0x1D

enum ReturnValues {SUCCESS, I2C_OPEN_ERR, I2C_ADDR_SET_ERR, FAIL_TO_WR_GYRO, FAIL_TO_RD_GYRO};

// Function prototypes
ReturnValues initI2C();
ReturnValues readITG3200(int file);
ReturnValues readITG3205(int file);

ReturnValues return_value;

int file;

#endif // GYRO_HPP