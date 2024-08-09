/**
 * @file main.cpp
 * @brief Main application for gyroscope data reading
 * @author Eren Gunduz
 * @date 05-08-2024
 */

#include "gyroscope.hpp"
#include <iostream>
#include <unistd.h>

using namespace std;
using namespace OBU;

int main()
{
    GyroOperations gyroOps;
    GyroscopeData gyroData;

    GyroError initResult = gyroOps.initialize();
    if (initResult != GyroError::SUCCESS)
    {
        cerr << "Failed to initialize gyroscope. Error code: " << static_cast<int>(initResult) << endl;
        return -1;
    }

    while (true)
    {
        gyroData = gyroOps.getGyroData();

        double x = gyroData.x;
        double y = gyroData.y;
        double z = gyroData.z;

        cout << "Gyro x: " << x << " Gyro y: " << y << " Gyro z: " << z << endl;

        usleep(1000000); // Sleep for 1 second
    }

    return 0;
}
