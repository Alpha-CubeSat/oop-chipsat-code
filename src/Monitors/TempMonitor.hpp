#ifndef TEMP_MONITOR_HPP_
#define TEMP_MONITOR_HPP_

#include "Wire.h"
#include "sfr.hpp"

class TempMonitor
{
public:
    TempMonitor();

    /**
     * @brief Reads data from the temperature sensor
     */
    void execute();

private:
    /**
     * @brief Buffer to store data read from the sensor
     */
    unsigned int data[2]; // TODO: uint32_t?
};

#endif
