#ifndef IMU_MONITOR_HPP_
#define IMU_MONITOR_HPP_

#include "Arduino_LSM9DS1.h"
#include "Modes/sensor_init_mode_type.enum"
#include "Modes/sensor_mode_type.enum"
#include "SensorReading.hpp"
#include "sfr.hpp"

class IMUMonitor
{
public:
    IMUMonitor();
    void execute();

private:
    void IMU_init();
    void transition_to_normal();
    void transition_to_abnormal_init();
    void capture_imu_values();

    sensor_mode_type mode;
    float gyroX, gyroY, gyroZ;
    float accX, accY, accZ;
};

#endif
