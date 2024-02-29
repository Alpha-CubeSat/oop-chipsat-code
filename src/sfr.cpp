#include "sfr.hpp"

namespace sfr {

    namespace imu {

        // TODO set min and max to good values
        // cubesat target spin is 5 rad/s -> 286 deg/s
        SensorReading *gyro_x = new SensorReading(-245, 245);
        SensorReading *gyro_y = new SensorReading(-245, 245);
        SensorReading *gyro_z = new SensorReading(-245, 245);

        SensorReading *acc_x = new SensorReading(-20, 20);
        SensorReading *acc_y = new SensorReading(-20, 20);
        SensorReading *acc_z = new SensorReading(-20, 20);

        bool initialized = false;
        uint16_t failed_times = 0;
        uint16_t failed_limit = 5;
        sensor_init_mode_type init_mode = sensor_init_mode_type::init;
        sensor_mode_type mode = sensor_mode_type::init;

    } // namespace imu

    namespace temperature {
        SensorReading *temp_c = new SensorReading(-40, 125);

    } // namespace temperature
    namespace gps {
        SensorReading *utc_time = new SensorReading(0, 235959.999);

        SensorReading *latitude = new SensorReading(-9000, 9000);
        SensorReading *longitude = new SensorReading(-18000, 18000);
        SensorReading *altitude = new SensorReading(0, 500000);

    } // namespace gps
    namespace radio {
        sensor_init_mode_type init_mode = sensor_init_mode_type::init;
        radio_mode_type mode = radio_mode_type::init;
        uint8_t start_progress = 0;
        uint32_t downlink_period = 30 * constants::time::one_second;
        uint32_t listen_period = 30 * constants::time::one_second;
        uint32_t command_wait_period = 30 * constants::time::one_second;
        uint32_t downlink_period_start;
        uint32_t listen_period_start;
        uint32_t command_wait_start;

    } // namespace radio

}; // namespace sfr
