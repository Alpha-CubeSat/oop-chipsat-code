#ifndef SFR_HPP_
#define SFR_HPP_

#include "Arduino.h"
#include "Modes/radio_mode_type.enum"
#include "Modes/sensor_init_mode_type.enum"
#include "Modes/sensor_mode_type.enum"
#include "constants.hpp"

namespace sfr {
    namespace imu {
        extern float gyro_x;
        extern float gyro_y;
        extern float gyro_z;

        extern float acc_x;
        extern float acc_y;
        extern float acc_z;

        extern bool initialized;
        extern uint16_t failed_times;
        extern uint16_t failed_limit;
        extern sensor_init_mode_type init_mode;
        extern sensor_mode_type mode;
    } // namespace imu
    namespace temperature {
        extern float temp_c;
    } // namespace temperature
    namespace radio {
        extern sensor_init_mode_type init_mode;
        extern uint8_t start_progress;
        extern radio_mode_type mode;

        extern uint32_t downlink_period;
        extern uint32_t listen_period;
        extern uint32_t command_wait_period;
        extern uint32_t downlink_period_start;
        extern uint32_t listen_period_start;
        extern uint32_t command_wait_start;

        extern uint8_t valid_uplinks;
        extern uint8_t invalid_uplinks;

    } // namespace radio
    namespace gps {
        extern float utc_time;

        extern float latitude;
        extern float longitude;
        extern float altitude;

        extern bool valid_msg;
        extern bool on;

        extern uint32_t boot_time;
    } // namespace gps

}; // namespace sfr

#endif