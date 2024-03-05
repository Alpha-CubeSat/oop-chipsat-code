#include "sfr.hpp"

namespace sfr {

    namespace imu {

        float gyro_x = -250;
        float gyro_y = 250;
        float gyro_z = 500;

        float acc_x = 30;
        float acc_y = -30;
        float acc_z = -50;

        bool initialized = false;
        uint16_t failed_times = 0;
        uint16_t failed_limit = 5;
        sensor_init_mode_type init_mode = sensor_init_mode_type::init;
        sensor_mode_type mode = sensor_mode_type::init;

    } // namespace imu

    namespace temperature {
        float temp_c;
    } // namespace temperature
    namespace gps {
        float utc_time;

        float latitude;
        float longitude;
        float altitude;

        bool valid_msg = false;

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
