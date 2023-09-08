#include "RadioMonitor.hpp"

uint8_t byte_counter = 0x00;

RadioMonitor::RadioMonitor()
{

    RFM96 local_radio = new Module(constants::radio::radio_cs_pin, constants::radio::radio_di0_pin, constants::radio::radio_rst_pin, constants::radio::radio_busy_pin);
    radio = &local_radio;
}

void RadioMonitor::init()
{
    if (sfr::radio::init_mode == sensor_init_mode_type::awaiting) {
        // Called camera_init function and initialization process has not yet started
        sfr::radio::init_mode = sensor_init_mode_type::in_progress;
    }

    if (sfr::radio::init_mode == sensor_init_mode_type::in_progress) {
        switch (sfr::radio::start_progress) {
        case 0:
            Serial.begin(115200);
            // initialize SX1278 with default settings
            Serial.print(F("[SX1278] Initializing ... "));
            code = radio->begin(constants::radio::freq, constants::radio::bw, constants::radio::sf, constants::radio::cr, constants::radio::sw, constants::radio::pwr, constants::radio::pl, constants::radio::gn);
            if (code == constants::radio::err_none) {
                Serial.println(F("success!"));
                sfr::radio::start_progress++;
            } else {
                Serial.print(F("failed, code "));
                Serial.println(code);
            }
            break;
        case 1:
            // saftey check to make sure frequency is set correctly
            Serial.print(F("[RF96] Setting frequency ... "));
            code = radio->setFrequency(constants::radio::freq);
            if (code == constants::radio::err_none) {
                Serial.println(F("success!"));
                sfr::radio::start_progress++;
            } else {
                Serial.print(F("failed, code "));
                Serial.println(code);
            }
            break;
        case 2:
            // adjust output power, avialable ranges: -3 to 15 dBm
            // increasing power increases range of transmission
            Serial.print(F("[RF96] Setting Output Power parameter ... "));
            code = radio->setOutputPower(constants::radio::pwr);
            if (code == constants::radio::err_none) {
                Serial.println(F("success!"));
                sfr::radio::start_progress++;
            } else {
                Serial.print(F("Output Power initialization error"));
                Serial.println(code);
            }
            break;
        case 3:
            // adjust spreading factor, avialable ranges: SF7 to SF12 (7 to 12)
            // increasing spreading factor increases range of transmission, but increases the time to transmit the message
            Serial.print(F("[RF96] Setting Spreading Factor parameter ... "));
            code = radio->setSpreadingFactor(constants::radio::sf);
            if (code == constants::radio::err_none) {
                Serial.println(F("success!"));
                sfr::radio::start_progress++;
            } else {
                Serial.print(F("Spreading Factor initialization error"));
                Serial.println(code);
            }
            break;
        case 4:
            // set CRC parameter to true so it matches the CRC parameter on the TinyGS side
            Serial.print(F("[RF96] Setting CRC parameter ... "));
            code = radio->setCRC(true);
            if (code == constants::radio::err_none) {
                Serial.println(F("success!"));
                sfr::radio::start_progress++;
            } else {
                Serial.print(F("CRC initialization error"));
                Serial.println(code);
            }
            break;
        case 5:
            // set forceLDRO parameter to true so it matches the forceLDRO parameter on the TinyGS side
            Serial.print(F("[RF96] Setting forceLDRO parameter ... "));
            code = radio->forceLDRO(true);
            if (code == constants::radio::err_none) {
                Serial.println(F("success!"));
                sfr::radio::start_progress++;
            } else {
                Serial.print(F("forceLDRO initialization error"));
                Serial.println(code);
            }

        case 6: // completed initialization
            sfr::radio::init_mode = sensor_init_mode_type::complete;
        }
    }
}

void RadioMonitor::transmit(byte byteArr[])
{
    code = radio->transmit(byteArr, 8);

    if (code == constants::radio::err_none) {
        // the packet was successfully transmitted
        Serial.println(F(" success!"));

        // print measured data rate
        Serial.print(F("[SX1278] Datarate:\t"));
        Serial.print(radio->getDataRate());
        Serial.println(F(" bps"));
    } else if (code == constants::radio::err_packet_too_long) {
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
    sfr::radio::mode = radio_mode_type::listen;
}

void RadioMonitor::receive()
{
    byte byteArr[8];
    code = radio->receive(byteArr, 8);

    if (code == RADIOLIB_ERR_NONE) {
        // packet was successfully received
        Serial.println(F("success!"));

        // print the data of the packet
        Serial.print(F("[SX1278] Data:\t\t\t"));
        // Serial.println(str);

        // print the RSSI (Received Signal Strength Indicator)
        // of the last received packet
        Serial.print(F("[SX1278] RSSI:\t\t\t"));
        Serial.print(radio->getRSSI());
        Serial.println(F(" dBm"));

        // print the SNR (Signal-to-Noise Ratio)
        // of the last received packet
        Serial.print(F("[SX1278] SNR:\t\t\t"));
        Serial.print(radio->getSNR());
        Serial.println(F(" dB"));

        // print frequency error
        // of the last received packet
        Serial.print(F("[SX1278] Frequency error:\t"));
        Serial.print(radio->getFrequencyError());
        Serial.println(F(" Hz"));

    } else if (code == constants::radio::err_rx_timeout) {
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
}

void RadioMonitor::execute()
{
    // just form report here to make life easy
    switch (sfr::radio::receive_mode)

        if (millis() - sfr::radio::last_receive >= sfr::radio::receive_period) {
            // receive every minutes- add bit to end of downlink report indicating we are in receive mode
            receive();
        }

    // transmit();
}

Command *RadioMonitor::commandFactory(RawCommand raw)
{
    // Create Specific Child Class of command depending on the OP Code
    uint16_t op_code = raw.get_f_opcode();

    // if (op_code == constants::radio::opcodes::downlink_period) {
    if (true) {
        Serial.println("Received new downlink period");
        return new SFROverrideCommand(raw);
    } else {

        Serial.print("Unknown Command with opcode: ");
        Serial.println(op_code, HEX);
        return new UnknownCommand(raw);
    }
}
