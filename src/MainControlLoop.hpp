#ifndef MAIN_CONTROL_LOOP_HPP_
#define MAIN_CONTROL_LOOP_HPP_

#include "ControlTasks/RadioControlTask.hpp"
#include "Monitors/TempMonitor.hpp"
#include "Monitors/IMUMonitor.hpp"
#include "Monitors/GPSMonitor.hpp"

class MainControlLoop
{
protected:
    IMUMonitor imu_monitor;
    TempMonitor temp_monitor;
    //GPSMonitor gps_monitor;
    RadioControlTask radio_control_task;

public:
    MainControlLoop();
    void execute();

private:
    float val;
};

#endif