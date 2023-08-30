#include "MainControlLoop.hpp"

MainControlLoop::MainControlLoop()
    : imu_monitor(),
      temp_monitor(),
      radio_monitor()

{
    delay(1000);
}

void MainControlLoop::execute()
{
    delay(200); // To prolong the speed of the main control loop to ensure correct RockBlock reads. Can reduce in the future.

    imu_monitor.execute();
    temp_monitor.execute();
    radio_monitor.execute();
}