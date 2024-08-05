/**
 * @file gyro.cpp
 * @brief Implementation of gyroscope functions
 * @author Eren Gunduz
 * @date 05-08-2024
 */

#include "gyro.hpp"
#include <unistd.h>
#include <iostream>

using namespace std;

/**
 * @brief Main function to initialize I2C and continuously read gyroscope data
 * @return int Return code indicating success or failure
 */
int main() {
    ReturnValues result = SUCCESS;
    
    // Initialize I2C
    if (initI2C() == SUCCESS) {
        // Read data from sensors and display on terminal
        while (result == SUCCESS) {
            result = readITG3200(fileDescriptor);
            if (result == SUCCESS) {
                result = readITG3205(fileDescriptor);
            }

            if (result == SUCCESS) {
                // Sleep for 1 second   
                usleep(1000000);
            }
        }
        close(fileDescriptor);
    } else {
        cerr << "Failed to initialize I2C." << endl;
        result = I2C_OPEN_ERR;
    }

    return result;
}