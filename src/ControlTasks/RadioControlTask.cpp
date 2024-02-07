#include "RadioControlTask.hpp"

RadioControlTask::RadioControlTask()
{
}

void RadioControlTask::init()
{
    if (sfr::radio::init_mode == sensor_init_mode_type::init) {
        switch (sfr::radio::start_progress) {
        case 0:
#ifdef VERBOSE
            Serial.print(F("Radio: Initializing ... "));
#endif
            // initialize SX1278 with default settings
            code = radio.begin(constants::radio::freq, constants::radio::bw, constants::radio::sf, constants::radio::cr,
                               constants::radio::sw, constants::radio::pwr, constants::radio::pl, constants::radio::gn);
            if (code == constants::radio::err_none) {
                sfr::radio::start_progress++;
            } else {
#ifdef VERBOSE
                Serial.print(F("failed, code "));
                Serial.println(code);
#endif
            }
            break;
        case 1:
#ifdef VERBOSE
            Serial.print(F("Radio: Setting frequency ... "));
#endif
            // check to make sure frequency is set correctly
            code = radio.setFrequency(constants::radio::freq);
            if (code == constants::radio::err_none) {
                sfr::radio::start_progress++;
            } else {
#ifdef VERBOSE
                Serial.print(F("failed, code "));
                Serial.println(code);
#endif
            }
            break;
        case 2:
#ifdef VERBOSE
            Serial.print(F("Radio: Setting Output Power parameter ... "));
#endif
            // adjust output power, avialable ranges: -3 to 15 dBm
            // increasing power increases range of transmission
            code = radio.setOutputPower(constants::radio::pwr);
            if (code == constants::radio::err_none) {
                sfr::radio::start_progress++;
            } else {
#ifdef VERBOSE
                Serial.print(F("failed, code "));
                Serial.println(code);
#endif
            }
            break;
        case 3:
#ifdef VERBOSE
            Serial.print(F("Radio: Setting Spreading Factor parameter ... "));
#endif
            // adjust spreading factor, avialable ranges: SF7 to SF12 (7 to 12)
            // increasing spreading factor increases range of transmission, but increases the time to transmit the message
            code = radio.setSpreadingFactor(constants::radio::sf);
            if (code == constants::radio::err_none) {
                sfr::radio::start_progress++;
            } else {
#ifdef VERBOSE
                Serial.print(F("failed, code "));
                Serial.println(code);
#endif
            }
            break;
        case 4:
#ifdef VERBOSE
            Serial.print(F("Radio: Setting CRC parameter ... "));
#endif
            // set CRC parameter to true so it matches the CRC parameter on the TinyGS side
            code = radio.setCRC(true);
            if (code == constants::radio::err_none) {
                sfr::radio::start_progress++;
            } else {
#ifdef VERBOSE
                Serial.print(F("failed, code "));
                Serial.println(code);
#endif
            }
            break;
        case 5:
#ifdef VERBOSE
            Serial.print(F("Radio: Setting forceLDRO parameter ... "));
#endif
            // set forceLDRO parameter to true so it matches the forceLDRO parameter on the TinyGS side
            code = radio.forceLDRO(true);
            if (code == constants::radio::err_none) {
                sfr::radio::start_progress++;
            } else {
#ifdef VERBOSE
                Serial.print(F("failed, code "));
                Serial.println(code);
#endif
            }
            break;
        case 6: // completed initialization
            sfr::radio::init_mode = sensor_init_mode_type::complete;
            break;
        }
    }
}

bool RadioControlTask::transmit(String packet)
{
    uint8_t buf[2];
    buf[0] = 'h';
    buf[1] = 'i';
    code = radio.transmit(buf, 2);

    if (code == constants::radio::err_none) {
        // the packet was successfully transmitted
#ifdef VERBOSE
        Serial.println(F("success!"));
        Serial.print(F("[SX1278] Datarate:\t"));
        Serial.print(radio.getDataRate());
        Serial.println(F(" bps"));
#endif
        return true;
    } else {
#ifdef VERBOSE
        if (code == constants::radio::err_packet_too_long) {
            // the supplied packet was longer than 256 bytes
            Serial.println(F("too long!"));
        } else if (code == constants::radio::err_tx_timeout) {
            // timeout occurred while transmitting packet
            Serial.println(F("timeout!"));
        } else {
            // some other error occurred
            Serial.print(F("failed, code "));
            Serial.println(code);
        }
#endif
        return false;
    }
}

bool RadioControlTask::receive()
{
    // don't use Arduino strings
    String command;
    // code = radio.receive(command);

    uint8_t str[8];
    code = radio.receive(str, sizeof(str));
    sfr::radio::received = command;

    if (code == constants::radio::err_none) {
#ifdef VERBOSE
        // packet was successfully received
        Serial.println(F("success!"));

        // print the data of the packet
        Serial.print(F("[SX1278] Data:\t\t\t"));

        // print the RSSI (Received Signal Strength Indicator)
        // of the last received packet
        Serial.print(F("[SX1278] RSSI:\t\t\t"));
        Serial.print(radio.getRSSI());
        Serial.println(F(" dBm"));

        // print the SNR (Signal-to-Noise Ratio)
        // of the last received packet
        Serial.print(F("[SX1278] SNR:\t\t\t"));
        Serial.print(radio.getSNR());
        Serial.println(F(" dB"));

        // print frequency error
        // of the last received packet
        Serial.print(F("[SX1278] Frequency error:\t"));
        Serial.print(radio.getFrequencyError());
        Serial.println(F(" Hz"));
#endif
        return true;
    } else {
#ifdef VERBOSE
        if (code == constants::radio::err_rx_timeout) {
            // timeout occurred while waiting for a packet
            Serial.println(F("timeout!"));
        } else if (code == constants::radio::err_crc_mismatch) {
            // packet was received, but is malformed
            Serial.println(F("CRC error!"));
        } else {
            // some other error occurred
            Serial.print(F("failed, code "));
            Serial.println(code);
        }
#endif
        return false;
    }
}

void RadioControlTask::execute()
{
    // implements the state machine described in: https://github.com/Alpha-CubeSat/oop-chipsat-code/wiki
    switch (sfr::radio::mode) {
    case radio_mode_type::init: {
#ifdef VERBOSE
        Serial.println(F("Radio: Init State"));
#endif
        init();
        if (sfr::radio::init_mode == sensor_init_mode_type::complete) {
            sfr::radio::mode = radio_mode_type::waiting;
            sfr::radio::listen_period_start = millis();
            sfr::radio::downlink_period_start = millis();
        }
        break;
    }
    case radio_mode_type::waiting: {
#ifdef VERBOSE
        Serial.println(F("Radio: Waiting State"));
#endif
        if (millis() - sfr::radio::listen_period_start >= sfr::radio::listen_period) {
            sfr::radio::mode = radio_mode_type::listen;
            sfr::radio::command_wait_start = millis();
        } else if (millis() - sfr::radio::downlink_period_start >= sfr::radio::downlink_period) {
            sfr::radio::mode = radio_mode_type::downlink;
        }
        break;
    }
    case radio_mode_type::downlink: {
#ifdef VERBOSE
        Serial.println(F("Radio: Downlink State"));
#endif
        // String normal_report = buildDownlink();
#ifdef VERBOSE
// Serial.println(normal_report);
#endif
        bool transmit_success = transmit(""); //transmit(normal_report);
        sfr::radio::mode = radio_mode_type::waiting;
        // reset downlink period if downlink successful
        if (transmit_success) {
            sfr::radio::downlink_period_start = millis();
        }
        break;
    }
    case radio_mode_type::listen: {
#ifdef VERBOSE
        Serial.println(F("Radio: Listen State"));
#endif
        // built in timeout is 100 LoRa symbols
        bool receive_success = receive();
#ifdef VERBOSE
        if (receive_success) {
            Serial.print(F("Received: "));
            Serial.println(sfr::radio::received);
        } else {
            Serial.println(F("Receive Failed"));
        }
#endif
        if (receive_success || millis() - sfr::radio::command_wait_start >= sfr::radio::command_wait_period) {
            sfr::radio::mode = radio_mode_type::waiting;
            sfr::radio::listen_period_start = millis();
        }
        break;
    }
    }
}

String RadioControlTask::buildDownlink()
{
    String packet = "";
    packet += sensorReadingString(sfr::imu::gyro_x) + ",";
    packet += sensorReadingString(sfr::imu::gyro_y) + ",";
    packet += sensorReadingString(sfr::imu::gyro_z) + ",";
    packet += sensorReadingString(sfr::imu::acc_x) + ",";
    packet += sensorReadingString(sfr::imu::acc_y) + ",";
    packet += sensorReadingString(sfr::imu::acc_z) + ",";
    packet += sensorReadingString(sfr::temperature::temp_c) + ",";
    // packet = sensorReadingString(sfr::gps::latitude) + ",";
    // packet += sensorReadingString(sfr::gps::longitude) + ",";
    packet += sensorReadingString(sfr::gps::altitude) + ",";
    // packet += sensorReadingString(sfr::gps::utc_h) + ",";
    // packet += sensorReadingString(sfr::gps::utc_m) + ",";
    // packet += sensorReadingString(sfr::gps::utc_s);
    return packet;
}

String RadioControlTask::sensorReadingString(SensorReading *sr)
{
    float val;
    sr->get_value(&val);
    return String(val);
}

// Command *RadioControlTask::commandFactory(RawCommand raw)
// {
//     // Create Specific Child Class of command depending on the OP Code
//     uint16_t op_code = raw.get_f_opcode();

//     // if (op_code == constants::radio::opcodes::downlink_period) {
//     if (true) {
//         Serial.println("Received new downlink period");
//         return new SFROverrideCommand(raw);
//     } else {

//         Serial.print("Unknown Command with opcode: ");
//         Serial.println(op_code, HEX);
//         return new UnknownCommand(raw);
//     }
// }