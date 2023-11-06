#ifndef MAIN_CONTROL_LOOP_HPP_
#define MAIN_CONTROL_LOOP_HPP_

#include "Monitors/TempMonitor.hpp"

class MainControlLoop
{
protected:
    TempMonitor temp_monitor;

public:
    MainControlLoop();
    void execute();

private:
    float val;
};

#endif