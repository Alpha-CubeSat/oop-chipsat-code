#ifndef MAIN_CONTROL_LOOP_HPP_
#define MAIN_CONTROL_LOOP_HPP_

#include "Monitors/TempMonitor.hpp"
#include "Monitors/IMUMonitor.hpp"

class MainControlLoop
{
protected:
    TempMonitor temp_monitor;
    IMUMonitor imu_monitor;

public:
    MainControlLoop();
    void execute();

private:
    float val;
};

#endif