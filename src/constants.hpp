#ifndef CONSTANTS_HPP_
#define CONSTANTS_HPP_

namespace constants {
    namespace time {
        constexpr unsigned long one_second = 1000;
        constexpr unsigned long one_minute = 60 * one_second;
        constexpr unsigned long one_hour = 60 * one_minute;
    } // namespace time
    namespace radio {
        constexpr int radio_cs_pin = 10;
        constexpr int radio_di0_pin = 2;
        constexpr int radio_rst_pin = 9;
        constexpr int radio_busy_pin = 16;

        constexpr float freq = 433.0;
        constexpr float bw = 125.0;
        constexpr int sf = 12;
        constexpr int cr = 5;
        constexpr int sw = 18;
        constexpr int pwr = 20;
        constexpr int pl = 8;
        constexpr int gn = 0;

    } // namespace radio
    namespace temperature {
        constexpr int i2c_address = 74;
    } // namespace temperature
    namespace gps {
        constexpr int rx_pin = 4;
        constexpr int tx_pin = 3;
    } // namespace gps

}; // namespace constants

#endif