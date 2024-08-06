/**
 * @file gyro.hpp
 * @brief Header file for GyroOperations class
 * @author Eren Gunduz
 * @date 05-08-2024
 */

#ifndef GYRO_HPP
#define GYRO_HPP


namespace GyroSensor 
{
    using namespace std;

/**
 * @brief I2C address of ITG3205 sensor
 */
#define SENSOR_ADDR 0x68

/**
 * @brief I2C bus device file
 */
#define I2C_BUS "/dev/i2c-1"

/**
 * @brief Register address for gyro X-axis high byte output
 */
#define ITG3200_REG_GYRO_XOUT_H 0x1D



/*!
    @brief Class for GyroOperations operations

*/

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

class GyroOperations {
public:

    /**
     * @brief Constructor for GyroOperations class
     * @details There is no connection at the beginning. 
     */
    GyroOperations();

    /**
     * @brief Destructor for GyroOperations class
     * @details It is used to close I2C communication 
     * after the end of operations. 
     */
    ~GyroOperations();

    /**
     * @brief Initialize I2C communication
     * @details Try to start I2C communication process. It returns success or fail. 
     * If it is not succesful, other processes will not be started.
     * @return Return SUCCESS or I2C_OPEN_ERR if I2C opening is failed.
     */
    ReturnValues initI2C();

    /**
     * @brief Read gyroscope data from ITG3205 sensor
     * @details This function reads the X, Y, and Z axis data from the ITG3205 gyroscope
     * sensor using the I2C interface. It first sets the I2C address of the sensor,
     * then writes the register address to initiate the read, and finally reads
     * the 6 bytes of data representing the gyroscope's X, Y, and Z axis values.
     * The register addresses for the ITG3205 are the same as for the ITG3200.
     * @return Return one of the status of ReturnValues enum.
     */
    ReturnValues readGyro();

private:
    int mFileDescriptor;/**< File descriptor for I2C communication */     
};

} // namespace GyroSensor

#endif // GYRO_HPP