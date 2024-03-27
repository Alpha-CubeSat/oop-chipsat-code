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

bool RadioControlTask::transmit(uint8_t *packet, uint8_t size)
{
    long start = millis();
    code = radio.transmit(packet, size);
    long time = millis() - start;
    Serial.print(F("Time to transmit (ms): "));
    Serial.println(time);

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
    uint8_t uplink[3];
    code = radio.receive(uplink, sizeof(uplink));
    received = uplink;

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
            sfr::radio::mode = radio_mode_type::downlink;
            sfr::radio::listen_period_start = millis();
            downlinkSettings();
        }
        break;
    }
    case radio_mode_type::downlink: {
#ifdef VERBOSE
        Serial.println(F("Radio: Downlink State"));
#endif
        // downlink when slot reached
        if (!sfr::radio::downlinked_in_slot && millis() - sfr::radio::downlink_window_start >= constants::radio::transmit_slot_length * sfr::radio::downlink_slot) {
            executeDownlink();
            sfr::radio::downlinked_in_slot = true;
        }

        // go into listen mode if listen period reached
        if (millis() - sfr::radio::listen_period_start >= constants::radio::listen_period) {
            sfr::radio::mode = radio_mode_type::listen;
            sfr::radio::command_wait_start = millis();

#ifdef VERBOSE
            Serial.println(F("Radio: Listen Flag Downlink"));
#endif
            // downlink with listen flag = true
            normalReportDownlink();
        }
        // reset window and choose slot for next downlink
        else if (millis() - sfr::radio::downlink_window_start >= sfr::radio::downlink_window_length) {
            downlinkSettings();
        }
        break;
    }
    case radio_mode_type::listen: {
#ifdef VERBOSE
        Serial.println(F("Radio: Listen State"));
#endif
        // built in timeout is 100 LoRa symbols
        bool receive_success = receive();
        if (receive_success) {
            processUplink();
        }

        if (receive_success || millis() - sfr::radio::command_wait_start >= constants::radio::command_wait_period) {
            sfr::radio::mode = radio_mode_type::downlink;
            sfr::radio::listen_period_start = millis();
            downlinkSettings();
        }
        break;
    }
    }
}

void RadioControlTask::downlinkSettings()
{
#ifdef VERBOSE
    Serial.println(F("Downlink Window Start"));
#endif
    // Reset window start time and pick random slot
    sfr::radio::downlink_window_start = millis();
    int num_slots = (sfr::radio::downlink_window_length / constants::radio::transmit_slot_length) - 1;
    sfr::radio::downlink_slot = random(0, num_slots + 1); // max is exclusive
    sfr::radio::downlinked_in_slot = false;
}

bool RadioControlTask::executeDownlink()
{
    // Downlink callsign report once in a while
    if (millis() - sfr::radio::last_callsign_time < constants::radio::callsign_interval) {
        return normalReportDownlink();
    } else {
        uint8_t dlink[] = {'K', 'D', '2', 'W', 'T', 'Q'};
        sfr::radio::last_callsign_time = millis();
#ifdef VERBOSE
        Serial.println(F("Callsign Report"));
#endif

        return transmit(dlink, sizeof(dlink));
    }
}

bool RadioControlTask::normalReportDownlink()
{
    // see https://github.com/Alpha-CubeSat/oop-chipsat-code/wiki/2.-Telemetry for more info
    uint16_t lat = sfr::gps::latitude * 10;
    uint16_t lon = sfr::gps::longitude * 10;
    uint16_t alt = sfr::gps::altitude / 10;

    uint8_t flags = 0;
    flags |= constants::radio::id << 6;
    flags |= sfr::gps::valid_msg << 5;                           // gps valid
    flags |= sfr::imu::initialized << 4;                         // imu valid
    flags |= sfr::gps::on << 3;                                  // boot mode flag
    flags |= (sfr::radio::mode == radio_mode_type::listen) << 2; // listen flag

    uint8_t dlink[] = {
        (uint8_t)(lat >> 8), (uint8_t)lat,
        (uint8_t)(lon >> 8), (uint8_t)lon,
        (uint8_t)(alt >> 8), (uint8_t)alt,
        map_range(sfr::imu::gyro_x, constants::imu::gyro_min, constants::imu::gyro_max),
        map_range(sfr::imu::gyro_y, constants::imu::gyro_min, constants::imu::gyro_max),
        map_range(sfr::imu::gyro_z, constants::imu::gyro_min, constants::imu::gyro_max),
        map_range(sfr::imu::acc_x, constants::imu::acc_min, constants::imu::acc_max),
        map_range(sfr::imu::acc_y, constants::imu::acc_min, constants::imu::acc_max),
        map_range(sfr::imu::acc_z, constants::imu::acc_min, constants::imu::acc_max),
        map_range(sfr::temperature::temp_c, constants::temperature::min, constants::temperature::max),
        (uint8_t)(sfr::radio::valid_uplinks << 4 | (sfr::radio::invalid_uplinks & 0x0F)),
        flags};

#ifdef VERBOSE
    Serial.println(F("Normal Report: "));
    for (size_t i = 0; i < sizeof(dlink); i++) {
        Serial.println(dlink[i]);
    }
#endif

    return transmit(dlink, sizeof(dlink));
}

uint8_t RadioControlTask::map_range(float value, int min_val, int max_val)
{
    long raw = map(value, min_val, max_val, 0, 255);
    raw = min(raw, 255);
    raw = max(raw, 0);
    return (uint8_t)raw;
}

void RadioControlTask::processUplink()
{
    // see https://github.com/Alpha-CubeSat/oop-chipsat-code/wiki/3.-Commands for more info
    switch (received[0]) {
    case constants::opcodes::no_op: // No-Op
#ifdef VERBOSE
        Serial.println(F("No-Op Command"));
#endif
        sfr::radio::valid_uplinks++;
        break;
    case constants::opcodes::change_downlink_window: { // Change downlink window length
        uint16_t combined = ((uint16_t)received[1] << 8) | received[2];
        sfr::radio::downlink_window_length = combined * constants::time::one_second;
        sfr::radio::valid_uplinks++;
#ifdef VERBOSE
        Serial.print(F("Change Downlink Command: "));
        Serial.println(combined);
#endif
        break;
    }
    default:
#ifdef VERBOSE
        Serial.println(F("Unknown Command"));
#endif
        sfr::radio::invalid_uplinks++;
        break;
    }
}