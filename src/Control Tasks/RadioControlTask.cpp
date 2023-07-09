#include "RadioControlTask.hpp"

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

RadioControlTask::RadioControlTask()
{

    RFM96 local_radio = new Module(RADIO_CS_PIN, RADIO_DI0_PIN, RADIO_RST_PIN, RADIO_BUSY_PIN);
    radio = &local_radio;
}

void RadioControlTask::Radio_init()
{
    /*if (sfr::imu::init_mode == sensor_init_mode_type::awaiting) {
        // Called imu_init function and initialization process has not yet started
        sfr::imu::init_mode = sensor_init_mode_type::in_progress;
    }

    if (sfr::imu::init_mode == sensor_init_mode_type::in_progress) {
        if (!IMU.begin()) {
            sfr::imu::init_mode = sensor_init_mode_type::failed;
        } else {
            sfr::imu::init_mode = sensor_init_mode_type::complete;
        }
    }*/

    Serial.begin(115200);
    // initialize SX1278 with default settings
    Serial.print(F("[SX1278] Initializing ... "));

    // begin(freq, bw, sf, cr, syncWord, power, preambleLength, gain)
    int state = radio->begin(freq, bw, sf, cr, sw, pwr, pl, gn);

    if (state == 0) { // ERR_NONE
        Serial.println(F("success!"));
    } else {
        Serial.print(F("failed, code "));
        Serial.println(state);
        while (true)
            ;
    }

    // saftey check to make sure frequency is at 436.7
    Serial.print(F("[RF96] Setting frequency ... "));
    state = radio->setFrequency(freq);
    if (state == 0) { // ERR_NONE
        Serial.println(F("success!"));
    } else {
        Serial.print(F("failed, code "));
        Serial.println(state);
        while (true)
            ;
    }

    // adjust output power, avialable ranges: -3 to 15 dBm
    // increasing power increases range of transmission
    Serial.print(F("[RF96] Setting Output Power parameter ... "));
    state = radio->setOutputPower(pwr);
    if (state == 0) { // ERR_NONE
        Serial.println(F("success!"));
    } else {
        Serial.print(F("Output Power initialization error"));
        Serial.println(state);
        while (true)
            ;
    }

    // adjust spreading factor, avialable ranges: SF7 to SF12 (7 to 12)
    // increasing spreading factor increases range of transmission, but increases the time to transmit the message
    Serial.print(F("[RF96] Setting Spreading Factor parameter ... "));
    state = radio->setSpreadingFactor(sf);
    if (state == 0) { // ERR_NONE
        Serial.println(F("success!"));
    } else {
        Serial.print(F("Spreading Factor initialization error"));
        Serial.println(state);
        while (true)
            ;
    }

    // set CRC parameter to true so it matches the CRC parameter on the TinyGS side
    Serial.print(F("[RF96] Setting CRC parameter ... "));
    state = radio->setCRC(true);
    if (state == 0) { // ERR_NONE
        Serial.println(F("success!"));
    } else {
        Serial.print(F("CRC initialization error"));
        Serial.println(state);
        while (true)
            ;
    }

    // set forceLDRO parameter to true so it matches the forceLDRO parameter on the TinyGS side
    Serial.print(F("[RF96] Setting forceLDRO parameter ... "));
    state = radio->forceLDRO(true); // FLDRO = 1 on TinyGS side
    if (state == 0) {               // ERR_NONE
        Serial.println(F("success!"));
    } else {
        Serial.print(F("forceLDRO initialization error"));
        Serial.println(state);
        while (true)
            ;
    }
    if (state == ERR_NONE) {
        Serial.println(F("success!"));
    } else {
        Serial.print(F("failed, code "));
        Serial.println(state);
        while (true)
            ;
    }
}

void RadioControlTask::execute()
{
}

Command *RadioControlTask::commandFactory(RawCommand raw)
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
