#include "RadioMonitor.hpp"

// put in constants
#define RADIO_CS_PIN 10
#define RADIO_DI0_PIN 2
#define RADIO_RST_PIN 9
#define RADIO_BUSY_PIN A2

#define ERR_NONE 0
#define ERR_CRC_MISMATCH -7
#define ERR_PACKET_TOO_LONG -4
#define ERR_TX_TIMEOUT -5

float freq = 433.0;
float bw = 125.0;
int sf = 12;
int cr = 5;
int sw = 18;
int pwr = 20;
int pl = 8;
int gn = 0;
uint8_t byte_counter = 0x00;

RadioMonitor::RadioMonitor()
{

    RFM96 local_radio = new Module(RADIO_CS_PIN, RADIO_DI0_PIN, RADIO_RST_PIN, RADIO_BUSY_PIN);
    radio = &local_radio;
}

void RadioMonitor::Radio_init()
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
            code = radio->begin(freq, bw, sf, cr, sw, pwr, pl, gn);
            if (code == ERR_NONE) {
                Serial.println(F("success!"));
                sfr::radio::start_progress++;
            } else {
                Serial.print(F("failed, code "));
                Serial.println(code);
            }
            break;
        case 1:
            // saftey check to make sure frequency is at 436.7
            Serial.print(F("[RF96] Setting frequency ... "));
            code = radio->setFrequency(freq);
            if (code == ERR_NONE) {
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
            code = radio->setOutputPower(pwr);
            if (code == ERR_NONE) {
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
            code = radio->setSpreadingFactor(sf);
            if (code == ERR_NONE) {
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
            if (code == ERR_NONE) { // ERR_NONE
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
            if (code == ERR_NONE) { // ERR_NONE
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

void RadioMonitor::transmit()
{
    code = radio->transmit(byteArr, 8);

    if (code == ERR_NONE) {
        // the packet was successfully transmitted
        Serial.println(F(" success!"));

        // print measured data rate
        Serial.print(F("[SX1278] Datarate:\t"));
        Serial.print(radio->getDataRate());
        Serial.println(F(" bps"));
    } else if (code == ERR_PACKET_TOO_LONG) {
        // the supplied packet was longer than 256 bytes
        Serial.println(F("too long!"));

    } else if (code == ERR_TX_TIMEOUT) {
        // timeout occurred while transmitting packet
        Serial.println(F("timeout!"));

    } else {
        // some other error occurred
        Serial.print(F("failed, code "));
        Serial.println(code);
    }
    sfr::radio::mode = radio_mode_type::receive;
}

void RadioMonitor::receive()
{
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

    } else if (code == RADIOLIB_ERR_RX_TIMEOUT) {
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
}

void RadioMonitor::execute()
{
    // just form report here to make life easy
    switch (sfr::radio_receive_mode)

        if (millis() - sfr::radio::last_receive >= sfr::radio::receive_period) {
            // receive every minutes- add bit to end of downlink report indicating we are in receive mode
            receive();
        }

    transmit();
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
