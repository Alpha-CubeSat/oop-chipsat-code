#include "sfr.hpp"

namespace sfr {
    namespace pins {
        arx::map<int, int> pinMap = {};

    } // namespace pins
    namespace imu {

        // TODO get min and max
        SensorReading *gyro_x = new SensorReading(0, 0);
        SensorReading *gyro_y = new SensorReading(0, 0);
        SensorReading *gyro_z = new SensorReading(0, 0);

        SensorReading *acc_x = new SensorReading(0, 0);
        SensorReading *acc_y = new SensorReading(0, 0);
        SensorReading *acc_z = new SensorReading(0, 0);

        bool powered = false;
        bool turn_on = true;
        uint16_t failed_times = 0;
        uint16_t failed_limit = 5;
        sensor_init_mode_type init_mode = sensor_init_mode_type::init;
        sensor_mode_type mode = sensor_mode_type::init;

    } // namespace imu
    namespace temp {
        SensorReading *temp_c = new SensorReading(-5000, 5000);
        SensorReading *temp_f = new SensorReading(-10000, 10000);

    } // namespace temp
    namespace radio {
        sensor_init_mode_type init_mode = sensor_init_mode_type::init;
        radio_mode_type mode = radio_mode_type::init;
        uint8_t start_progress = 0;
        uint32_t downlink_period = 5 * constants::time::one_minute;
        uint32_t listen_period = 5 * constants::time::one_minute;
        uint32_t command_wait_period = 5 * constants::time::one_minute;
        uint32_t downlink_period_start;
        uint32_t listen_period_start;
        uint32_t command_wait_start;

    } // namespace radio

    namespace gps {
        SensorReading *utc_h = new SensorReading(0, 0);
        SensorReading *utc_m = new SensorReading(0, 0);
        SensorReading *utc_s = new SensorReading(0, 0);

        SensorReading *latitude = new SensorReading(0, 0);
        SensorReading *longitude = new SensorReading(0, 0);
        SensorReading *altitude = new SensorReading(0, 0);

        // extra slot for '\0' when tokenizing
        char watch_dog[constants::gps::dog_size + 1];
    } // namespace gps

}; // namespace sfr
