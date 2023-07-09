#include "sfr.hpp"

namespace sfr {
    namespace pins {
        // Dictionary<int, int> pinMap = {};
        // Dictionary *pinMap = new Dictionary();

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
        sensor_init_mode_type init_mode = sensor_init_mode_type::awaiting;
        sensor_mode_type mode = sensor_mode_type::init;

    } // namespace imu

    namespace temp {
        SensorReading *temp_c = new SensorReading(-5000, 5000);
        SensorReading *temp_f = new SensorReading(-10000, 10000);

    } // namespace temp

    namespace eeprom {
        uint16_t sfr_address = 7;
        uint8_t boot_counter = 0; // fix this

    } // namespace eeprom

}; // namespace sfr
