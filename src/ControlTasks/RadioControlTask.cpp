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
            if (code == RADIOLIB_ERR_NONE) {
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
            if (code == RADIOLIB_ERR_NONE) {
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
            if (code == RADIOLIB_ERR_NONE) {
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
            if (code == RADIOLIB_ERR_NONE) {
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
            if (code == RADIOLIB_ERR_NONE) {
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
            if (code == RADIOLIB_ERR_NONE) {
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

bool RadioControlTask::transmit(uint8_t *packet)
{
    code = radio.transmit(packet, 12);

    if (code == RADIOLIB_ERR_NONE) {
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
        if (code == RADIOLIB_ERR_PACKET_TOO_LONG) {
            // the supplied packet was longer than 256 bytes
            Serial.println(F("too long!"));
        } else if (code == RADIOLIB_ERR_TX_TIMEOUT) {
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
    uint8_t str[3];
    code = radio.receive(str, sizeof(str));
    if (code == RADIOLIB_ERR_NONE) {
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
        switch (str[0]) {
        case constants::opcodes::no_op: // No-Op
            break;
        case constants::opcodes::change_downlink_period: // Change downlink period
            uint16_t combined = (static_cast<uint16_t>(str[1]) << 8) | str[2];
            sfr::radio::downlink_period = combined * constants::time::one_second;
            break;
        default:
            break;
        }
        return true;
    } else {
#ifdef VERBOSE
        if (code == RADIOLIB_ERR_RX_TIMEOUT) {
            // timeout occurred while waiting for a packet
            Serial.println(F("timeout!"));
        } else if (code == RADIOLIB_ERR_CRC_MISMATCH) {
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
        bool transmit_success = executeDownlink();
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

bool RadioControlTask::executeDownlink()
{
    uint8_t dlink[] = {
        serialize(sfr::gps::altitude),
        serialize(sfr::gps::latitude),
        serialize(sfr::gps::longitude),
        serialize(sfr::gps::utc_time),
        serialize(sfr::imu::acc_x),
        serialize(sfr::imu::acc_y),
        serialize(sfr::imu::acc_z),
        serialize(sfr::imu::gyro_x),
        serialize(sfr::imu::gyro_y),
        serialize(sfr::imu::gyro_z),
        serialize(sfr::temperature::temp_c),
        sfr::radio::mode == radio_mode_type::listen};
    return transmit(dlink);
}

uint8_t RadioControlTask::serialize(SensorReading *valueObj)
{
    float value;
    valueObj->get_value(&value);
    return round(map(value, valueObj->get_min(), valueObj->get_max(), 0, 255));
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