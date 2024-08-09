/**
 * @file gyroscope.hpp
 * @brief Header file for Gyroscope operations
 * @author Eren Gunduz
 * @date 05-08-2024
 */

#ifndef GYROSCOPE_HPP
#define GYROSCOPE_HPP

#include <string>

/**
 * @namespace OBU
 * @brief Namespace for OBU-related classes and functions
 */
namespace OBU {

    using namespace std;

/**
 * @enum GyroError
 * @brief Enumeration of return values for function calls
 */
enum class GyroError {
    SUCCESS,          /**< Operation completed successfully */
    I2C_OPEN_ERR,     /**< Error opening I2C bus */
    I2C_ADDR_SET_ERR, /**< Error setting I2C slave address */
    SENSOR_WAKE_FAIL, /**< Error setting power mode */
    I2C_WRITE_ERR,
    I2C_READ_ERR
};

/**
 * @struct GyroscopeData
 * @brief Struct to store gyroscope data for X, Y, Z axes
 */
struct GyroscopeData {
    double x;
    double y;
    double z;
};

/**
 * @class GyroOperations
 * @brief Class for managing gyroscope operations
 */
class GyroOperations {
public:

    /**
     * @brief Constructor for GyroOperations class
     * @details There is no connection at the beginning.
     */
    GyroOperations();

    /**
     * @brief Destructor for GyroOperations class
     * @details Closes I2C communication after the end of operations.
     */
    ~GyroOperations();

    /**
     * @brief Initialize I2C communication
     * @details Tries to start I2C communication process. If it is not successful, other processes will not be started.
     * @return GyroError::SUCCESS or GyroError::I2C_OPEN_ERR if I2C opening fails.
     */
    GyroError initialize();

    /**
     * @brief Read gyroscope data from sensor
     * @details Reads the X, Y, and Z axis data as degree per second from the gyroscope sensor using the I2C interface.
     * @return GyroError status of the operation.
     */
    GyroscopeData getGyroData();

private:
    string device;                                      /**< I2C device file */
    int mFileDescriptor;                                /**< File descriptor for I2C communication */
    static const uint8_t SENSOR_ADDR;                   /**< I2C address of sensor */
    static const string I2C_BUS;                        /**< I2C bus device file */
    static const uint8_t SENSOR_REG_GYRO_XOUT_H;        /**< Register address for gyro X-axis high byte output */
    static const uint8_t SENSOR_REG_POWER_CTL;
    static const uint8_t SENSOR_REG_DATAX0;
    static const uint8_t SENSOR_MEASURE_MODE;
    static const int NO_FILE_FD;
    static const int FILE_OP_VALUE;
    static const int GYRO_WR_REG;
    static const int SHIFT_VALUE;
    static const size_t BUFFER_SIZE;
    static const size_t WR_REG_BUFFER_SIZE;
    static const int X_AXIS_START_VALUE;
    static const int Y_AXIS_START_VALUE;
    static const int Z_AXIS_START_VALUE;
    static const int BUFFER_START_VALUE;

    /**
     * @brief Reads gyroscope data from the sensor
     * @param x Reference to store X-axis data
     * @param y Reference to store Y-axis data
     * @param z Reference to store Z-axis data
     * @return GyroError status of the operation
     */
   GyroError readGyro(double& x, double& y, double& z);

    /**
     * @brief Writes a value to a gyroscope register
     * @param reg Register address
     * @param value Value to write
     * @return GyroError status of the operation
     */
    GyroError writeRegister(uint8_t reg, uint8_t value);

    /**
     * @brief Reads values from gyroscope registers
     * @param reg Register address
     * @param buffer Buffer to store the read data
     * @param length Number of bytes to read
     * @return GyroError status of the operation
     */
    GyroError readRegisters(uint8_t reg, uint8_t* buffer, size_t length);

    double scaleFactor = 131.0; /**< Scale factor for MPU6050 for converting raw data to degrees per second */
};

} // namespace OBU

#endif // GYROSCOPE_HPP
