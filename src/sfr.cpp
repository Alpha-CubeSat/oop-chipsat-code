#include "sfr.hpp"

namespace sfr {
    namespace imu {
        bool initialized = false;

        float gyro_x, gyro_y, gyro_z;
        float acc_x, acc_y, acc_z;
        float mag_x, mag_y, mag_z;

    } // namespace imu
    namespace temperature {
        bool initialized = false;

        float temp_c;
    } // namespace temperature
    namespace gps {
        float utc_time, latitude, longitude, altitude;
        bool valid_location = false;
        bool valid_altitude = false;
        bool on = false;

        uint32_t boot_time = 0;

    } // namespace gps
    namespace radio {
        bool initialized = false;
        radio_mode_type mode = radio_mode_type::init;
        uint8_t start_progress = 0;
        uint8_t alive_signal_dlinks = 0;

        // This must be a multiple of constants::radio::transmit_slot_length
        uint32_t downlink_window_length = 3500; // ms
        uint32_t downlink_window_start;
        uint32_t listen_period_start;
        uint32_t command_wait_start;
        uint32_t last_callsign_time;

        uint8_t downlink_slot;
        bool downlinked_in_slot = false;

        uint8_t valid_uplinks = 0;
        uint8_t invalid_uplinks = 0;

    } // namespace radio
};    // namespace sfr
