#ifndef CONSTANTS_HPP_
#define CONSTANTS_HPP_

namespace constants {
    namespace time {
        constexpr uint32_t one_second = 1000;
        constexpr uint32_t one_minute = 60 * one_second;
        constexpr uint32_t one_hour = 60 * one_minute;
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

#ifdef CHIPSAT_ID
        constexpr uint8_t id = CHIPSAT_ID;
#else
        constexpr uint8_t id = 0;
#endif

        constexpr uint32_t listen_period = 30 * constants::time::one_second;
        constexpr uint32_t command_wait_period = 30 * constants::time::one_second;
        constexpr uint32_t callsign_interval = 10 * constants::time::one_minute;

        constexpr uint32_t transmit_slot_length = 2 * constants::time::one_second;

    } // namespace radio
    namespace imu {
        constexpr int gyro_min = -245;
        constexpr int gyro_max = 245;
        constexpr int acc_min = -20;
        constexpr int acc_max = 20;
    } // namespace imu
    namespace temperature {
        constexpr int i2c_address = 74;

        constexpr int min = -40;
        constexpr int max = 125;
    } // namespace temperature
    namespace gps {
        constexpr int rx_pin = 4;
        constexpr int tx_pin = 3;
        constexpr int reset_pin = 7;
        constexpr int buffer_size = 11;

        constexpr int lat_min = -9000; // In (10 * degrees)
        constexpr int lat_max = 9000;
        constexpr int lon_min = -18000; // In (10 * degrees)
        constexpr int lon_max = 18000;
        constexpr int alt_min = 0;
        constexpr int alt_max = 50000; // In 10's of meters
    }                                  // namespace gps
    namespace opcodes {
        constexpr uint8_t no_op = 0x00;
        constexpr uint8_t change_downlink_window = 0x11;
    } // namespace opcodes

}; // namespace constants

#endif