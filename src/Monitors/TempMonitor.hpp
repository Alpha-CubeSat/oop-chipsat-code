#ifndef TEMP_MONITOR_HPP_
#define TEMP_MONITOR_HPP_

#include "Wire.h"
#include "sfr.hpp"

class TempMonitor
{
public:
    TempMonitor();
    void execute();

private:
    bool initialized = false;
};

#endif
