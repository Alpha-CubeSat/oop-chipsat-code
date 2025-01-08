#ifndef IMU_MONITOR_HPP_
#define IMU_MONITOR_HPP_

#include "Arduino_LSM9DS1.h"
#include "sfr.hpp"

class IMUMonitor
{
public:
    IMUMonitor();

    /**
     * @brief Initializes the IMU once and reads IMU data every time after
     */
    void execute();

private:
    /**
     * @brief Reads IMU data
     */
    void capture_imu_values();

    LSM9DS1Class imu;
};

#endif
