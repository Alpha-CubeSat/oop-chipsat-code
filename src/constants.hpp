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
    namespace radio {
        constexpr int radio_cs_pin = 10;
        constexpr int radio_di0_pin = 2;
        constexpr int radio_rst_pin = 9;
        constexpr int radio_busy_pin = A2;

        constexpr int err_none = 0;
        constexpr int err_crc_mismatch = -7;
        constexpr int err_packet_too_long = -4;
        constexpr int err_tx_timeout = -5;
        constexpr int err_rx_timeout = -6;

        constexpr float freq = 433.0;
        constexpr float bw = 125.0;
        constexpr int sf = 12;
        constexpr int cr = 5;
        constexpr int sw = 18;
        constexpr int pwr = 20;
        constexpr int pl = 8;
        constexpr int gn = 0;

        // constexpr size_t buffer_size = 63;
        // constexpr size_t packet_size = 70;
        // constexpr size_t num_commas = 5;
        // constexpr size_t opcode_len = 2;
        // constexpr size_t arg1_len = 4;
        // constexpr size_t arg2_len = 4;
        // constexpr size_t command_len = opcode_len + arg1_len + arg2_len;
        // constexpr size_t max_conseq_read = 3;

        // constexpr uint8_t end_of_normal_downlink_flag1 = 254;
        // constexpr uint8_t end_of_normal_downlink_flag2 = 255;
        // constexpr uint8_t end_of_command_upload_flag1 = 0;
        // constexpr uint8_t end_of_command_upload_flag2 = 250;
        // namespace opcodes {
        //     // SFRField Commands
        //     constexpr uint16_t sfr_field_opcode_min = 0x1100;
        //     constexpr uint16_t sfr_field_opcode_max = 0x2899;

        //     // Deploy Command
        //     constexpr uint16_t sfr_field_opcode_deploy = 0x3333;

        //     // Arm Command
        //     constexpr uint16_t sfr_field_opcode_arm = 0x4444;

        //     // Fire Command
        //     constexpr uint16_t sfr_field_opcode_fire = 0x5555;

        // } // namespace opcodes
    } // namespace radio
    
    namespace temperature {
        constexpr int i2c_address = 0x4A;
    } // namespace temperature
    namespace imu {

    } // namespace imu
    namespace gps {
        constexpr int dog_size = 62;
    } // namespace gps

    
}; // namespace constants

#endif