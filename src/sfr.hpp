#ifndef SFR_HPP_
#define SFR_HPP_

#include "Arduino.h"
#include "Modes/radio_mode_type.enum"
#include "Modes/sensor_init_mode_type.enum"
#include "Modes/sensor_mode_type.enum"
#include "SensorReading.hpp"
#include "constants.hpp"

namespace sfr {
    namespace imu {
        extern SensorReading *gyro_x;
        extern SensorReading *gyro_y;
        extern SensorReading *gyro_z;

        extern SensorReading *acc_x;
        extern SensorReading *acc_y;
        extern SensorReading *acc_z;

        extern bool initialized;
        extern uint16_t failed_times;
        extern uint16_t failed_limit;
        extern sensor_init_mode_type init_mode;
        extern sensor_mode_type mode;
    } // namespace imu
    namespace temperature {
        extern SensorReading *temp_c;
        extern SensorReading *temp_f;
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
        extern String received;

    } // namespace radio
    namespace gps {
        extern SensorReading *utc_h;
        extern SensorReading *utc_m;
        extern SensorReading *utc_s;

        extern SensorReading *latitude;
        extern SensorReading *longitude;
        extern SensorReading *altitude;

        extern char watch_dog[constants::gps::dog_size + 1];
    } // namespace gps

}; // namespace sfr

#endif