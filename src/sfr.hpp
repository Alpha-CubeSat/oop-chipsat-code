#ifndef SFR_HPP_
#define SFR_HPP_

#include "Arduino.h"
#include "Modes/sensor_init_mode_type.enum"
#include "Modes/sensor_mode_type.enum"
#include "SFRField.hpp"
#include "SensorReading.hpp"
#include "constants.hpp"
#include <ArxContainer.h>

namespace sfr {
    namespace pins {
        extern arx::map<int, int> pinMap;
    } // namespace pins

    namespace imu {
        extern SensorReading *gyro_x;
        extern SensorReading *gyro_y;
        extern SensorReading *gyro_z;

        extern SensorReading *acc_x;
        extern SensorReading *acc_y;
        extern SensorReading *acc_z;

        extern bool powered;
        extern bool turn_on;
        extern uint16_t failed_times;
        extern uint16_t failed_limit;
        extern sensor_init_mode_type init_mode;
        extern sensor_mode_type mode;
    } // namespace imu

    namespace temp {
        extern SensorReading *temp_c;
        extern SensorReading *temp_f;
    } // namespace temp

    namespace eeprom {
        extern uint16_t sfr_address;
        extern uint8_t boot_counter;
    } // namespace eeprom

    namespace radio {
        extern sensor_init_mode_type init_mode;
        extern uint8_t start_progress;
        extern bool receive_mode;
        extern uint32_t receive_period;

    } // namespace radio
};    // namespace sfr

#endif