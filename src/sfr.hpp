#ifndef SFR_HPP_
#define SFR_HPP_

#include "Arduino.h"
#include "Modes/radio_mode_type.enum"
#include "constants.hpp"

namespace sfr {
    namespace imu {
        extern bool initialized;

        extern float gyro_x;
        extern float gyro_y;
        extern float gyro_z;

        extern float acc_x;
        extern float acc_y;
        extern float acc_z;

        extern float mag_x;
        extern float mag_y;
        extern float mag_z;

    } // namespace imu
    namespace temperature {
        extern bool initialized;

        extern float temp_c;
    } // namespace temperature
    namespace radio {
        extern bool initialized;
        extern radio_mode_type mode;
        extern uint8_t start_progress;
        extern uint8_t alive_signal_dlinks;

        extern uint32_t downlink_window_length;
        extern uint32_t downlink_window_start;
        extern uint32_t listen_period_start;
        extern uint32_t command_wait_start;
        extern uint32_t last_callsign_time;

        extern uint8_t downlink_slot;
        extern bool downlinked_in_slot;

        extern uint8_t valid_uplinks;
        extern uint8_t invalid_uplinks;

    } // namespace radio
    namespace gps {
        extern float utc_time;

        extern float latitude;
        extern float longitude;
        extern float altitude;

        extern bool valid_location;
        extern bool valid_altitude;
        extern bool on;

        extern uint32_t boot_time;
    } // namespace gps
};    // namespace sfr

#endif