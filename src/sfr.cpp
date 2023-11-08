#include "sfr.hpp"

namespace sfr {
    
    namespace imu {

        // TODO set min and max to good values
        SensorReading *gyro_x = new SensorReading(-2550, 2550);
        SensorReading *gyro_y = new SensorReading(-2550, 2550);
        SensorReading *gyro_z = new SensorReading(-2550, 2550);

        SensorReading *acc_x = new SensorReading(-2550, 2550);
        SensorReading *acc_y = new SensorReading(-2550, 2550);
        SensorReading *acc_z = new SensorReading(-2550, 2550);

        bool initialized = false;
        uint16_t failed_times = 0;
        uint16_t failed_limit = 5;
        sensor_init_mode_type init_mode = sensor_init_mode_type::init;
        sensor_mode_type mode = sensor_mode_type::init;

    } // namespace imu

    namespace temperature {
        SensorReading *temp_c = new SensorReading(-5000, 5000);
        SensorReading *temp_f = new SensorReading(-10000, 10000);

    } // namespace temperature
    
}; // namespace sfr
