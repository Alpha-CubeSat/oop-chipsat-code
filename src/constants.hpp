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
        constexpr int radio_rst_pin = 5;
        constexpr int radio_busy_pin = 16;

        constexpr float freq = 437.4; // MHz
        constexpr float bw = 62.5;    // kHz
        constexpr int sf = 10;        // Between 7 and 12
        constexpr int cr = 5;         // Between 5 and 8. 4/8 coding ratio. One redundancy bit for every data bit
        constexpr int sw = 18;        // Sync-word (defines network). Default is 0d18
        constexpr int pwr = 20;       // Between 2 and 17, or 20 for max power
        constexpr int pl = 8;         // Payload length
        constexpr int gn = 0;         // Gain

        constexpr uint8_t max_alive_signal_dlinks = 3;

#ifdef CHIPSAT_ID
        constexpr uint8_t id = CHIPSAT_ID;
#else
        constexpr uint8_t id = 0;
#endif

        constexpr uint32_t listen_period = 30 * constants::time::one_minute;
        constexpr uint32_t command_wait_period = 30 * constants::time::one_second;
        constexpr uint32_t callsign_interval = 10 * constants::time::one_minute;

        constexpr uint32_t transmit_slot_length = 700; // ms
        // TODO: Verify sensor data on alive signal
    } // namespace radio
    namespace imu {
        constexpr int gyro_min = -245; // Default gyro range is +/- 245 dps
        constexpr int gyro_max = 245;
        constexpr int acc_min = -10; // Default accel range is +/- 2 G
        constexpr int acc_max = 10;
        constexpr int mag_min = -100; // Default mag range is +/- 4 gauss -> 100 uT
        constexpr int mag_max = 100;
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

        constexpr uint32_t boot_time = 10 * constants::time::one_second;
    } // namespace gps
    namespace opcodes {
        constexpr uint8_t no_op = 0x00;
        constexpr uint8_t change_downlink_window = 0x11;
    } // namespace opcodes
    namespace led {
        constexpr int led_pin = 9;
        constexpr uint32_t led_on_time = 5 * constants::time::one_second;
    } // namespace led
};    // namespace constants

#endif