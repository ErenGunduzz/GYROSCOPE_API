/**
 * @file main.cpp
 * @brief Main application for gyroscope data reading
 * @author Eren Gunduz
 * @date 05-08-2024
 */

#include "gyro.hpp"
#include <unistd.h>
#include <iostream>

using namespace GyroSensor;
using namespace std;

int main() {
    Gyroscope gyro;
    Gyroscope::ReturnValues result;
    
    // Initialize I2C
    result = gyro.initI2C();
    if (result == Gyroscope::SUCCESS) {
        // Read data from sensors and display on terminal
        while (result == Gyroscope::SUCCESS) {
            result = gyro.readITG3200();
            if (result == Gyroscope::SUCCESS) {
                result = gyro.readITG3205();
            }

            if (result == Gyroscope::SUCCESS) {
                // Sleep for 1 second   
                usleep(1000000);
            }
        }
    } else {
        std::cerr << "Failed to initialize I2C." << std::endl;
    }

    return result;
}