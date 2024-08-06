/**
 * @file main.cpp
 * @brief Main application for gyroscope data reading
 * @author Eren Gunduz
 * @date 05-08-2024
 */

#include "gyro.hpp"
#include <unistd.h>
#include <iostream>

static const int SLEEP_TIME = 1000000; // as microseconds

using namespace GyroSensor;
using namespace std;

int main() 
{
    GyroOperations gyro;
    ReturnValues result;
    
    // Initialize I2C
    result = gyro.initI2C();
    if (result == SUCCESS) 
    {
        // Read data from sensors and display on terminal
        while (result == SUCCESS) 
        {
            result = gyro.readGyro();

            if (result == SUCCESS) 
            {
                // Sleep for 1 second   
                usleep(SLEEP_TIME);
            }
            else 
            {
                cerr << "Failed to read GyroOperations data." << endl;
            }
        }
    } 
    else 
    {
        cerr << "Failed to initialize I2C." << endl;
    }

    return result;
}