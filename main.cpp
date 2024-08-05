#include "gyro.hpp"
#include <unistd.h>
#include <iostream>

using namespace std;

int main() {
    // start I2C
    file = initI2C();
    if (file < 0) {
        cerr << "I2C not started." << endl;
        return 1;
    }

    // Read data from sensors and display on terminal, write to a file
    while (true) {
        readITG3200(file);
        readITG3205(file);

        // Sleep for 100 milliseconds
        usleep(100000);
    }

    close(file);
    return 0;
}