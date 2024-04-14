#ifndef IMU_MONITOR_HPP_
#define IMU_MONITOR_HPP_

#include "Arduino_LSM9DS1.h"
#include "Modes/sensor_init_mode_type.enum"
#include "Modes/sensor_mode_type.enum"
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
     * @brief Initializes the IMU
     */
    void init();

    /**
     * @brief Transitions to a normal state (TODO: Unnecessary?)
     */
    void transition_to_normal();

    /**
     * @brief Transitions to a abnormal state (TODO: Unnecessary?)
     */
    void transition_to_abnormal_init();

    /**
     * @brief Reads IMU data
     */
    void capture_imu_values();

    /**
     * @brief The current state of the sensor
     */
    sensor_mode_type mode;
};

#endif
