#ifndef CONSTANTS_HPP_
#define CONSTANTS_HPP_

#include <stdint.h>

namespace constants {
    namespace time {
        constexpr unsigned long one_second = 1000;
        constexpr unsigned long one_minute = 60 * one_second;
        constexpr unsigned long one_hour = 60 * one_minute;
        constexpr unsigned long one_day = 24 * one_hour;
        constexpr unsigned long half_second = one_second / 2;
    } // namespace time
    namespace sensor {
        constexpr int collect = 3;
        constexpr int repeats = 10;
    } // namespace sensor
    
    namespace temperature {
        constexpr int i2c_address = 0x4A;
    } // namespace temperature
    namespace imu {

    } // namespace imu

    
}; // namespace constants

#endif