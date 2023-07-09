#ifndef MAIN_CONTROL_LOOP_HPP_
#define MAIN_CONTROL_LOOP_HPP_

#include "Monitors/IMUMonitor.hpp"
#include "Monitors/TempMonitor.hpp"

class MainControlLoop
{
protected:
    IMUMonitor imu_monitor;
    TempMonitor temp_monitor;

public:
    MainControlLoop();
    void execute();
};

#endif