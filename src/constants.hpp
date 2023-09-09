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
        constexpr int pin = 39;
        constexpr float in_sun_val = 30;
    } // namespace temperature
    namespace imu {

    } // namespace imu
    namespace eeprom {
        // EEPROM SFR memory offsets, determined by size of the prevoius SFR value type + size of a boolean (1 byte) + offset of the previous SFR value
        // The boolean indicates whether or not to restore the value on bootup
        static constexpr unsigned int eeprom_boot_count_offset = 0;
        static constexpr unsigned int eeprom_wait_time_write_step_time_offset = eeprom_boot_count_offset + 2;
        static constexpr unsigned int eeprom_alloted_time_offset = eeprom_wait_time_write_step_time_offset + 5;
        static constexpr unsigned int eeprom_alloted_time_passed_offset = eeprom_alloted_time_offset + 5;
        static constexpr unsigned int eeprom_sfr_write_step_time_offset = eeprom_alloted_time_passed_offset + 2;
        static constexpr unsigned int eeprom_sfr_address_age_offset = eeprom_sfr_write_step_time_offset + 5;
        static constexpr unsigned int eeprom_storage_full_offset = eeprom_sfr_address_age_offset + 5;

        static constexpr unsigned int full_offset = eeprom_storage_full_offset + 2;
    } // namespace eeprom
};    // namespace constants

#endif