/**
 * @file main.cpp
 * @brief Main application for gyroscope data reading
 * @author Eren Gunduz
 * @date 05-08-2024
 */

#include "gyroscope.hpp"
#include <iostream>

using namespace std;
using namespace OBU;

int main()
{
    GyroOperations gyroOps;
    GyroscopeData gyroData;

    GyroError initResult = gyroOps.initialize();
    if (initResult != GyroError::SUCCESS)
    {
        cerr << "Failed to initialize gyrsocope. Error code: " << static_cast<int>(initResult) << endl;
        return -1;
    }

    gyroData = gyroOps.getGyroData();

    double x = static_cast<double>(gyroData.x);
    double y = static_cast<double>(gyroData.y);
    double z = static_cast<double>(gyroData.z);

    cout << "Gyro x: " << x << " Gyro y: " << y << " Gyro z:  " << z << endl;

    return 0;
}