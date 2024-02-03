#include <Arduino.h>
#include <SoftwareSerial.h>
#include <string.h>

#include "GPSMonitor.hpp"

GPSMonitor::GPSMonitor()
{
    uint8_t SetNMEA[] = {
        0xA0, 0xA1, 0x00, 0x03, 0x09, 0x01, 0x01, 0x09, 0x0D, 0x0A};

    ss.begin(115200);
    // delay(1000);
    ss.write((uint8_t *)&SetNMEA, sizeof(SetNMEA));
    // delay(500);
    // ss.write((uint8_t *)&SetNMEA, sizeof(SetNMEA));
    // delay(500);
    // ss.write((uint8_t *)&SetNMEA, sizeof(SetNMEA));
    // delay(1000);

    sfr::gps::altitude->set_valid();
}

// return true at end of term
bool GPSMonitor::encode(char c) {
    
    switch (c) {
    case ',':
        // clear term buffer and udpate sfr if needed

        switch (term_count) {
        case 2: // UTC
            memcpy(&sfr::gps::utc_time, &term_buffer, 10);
            break;
        case 3: // Latitude value
            memcpy(&sfr::gps::latitude, &term_buffer, char_count);
            break;
        case 4: // Latitude hemisphere
            sfr::gps::latitude[char_count] = term_buffer[0]; // TODO edge case
            break;
        case 5: // Longitude value
            memcpy(&sfr::gps::longitude, &term_buffer, char_count);
            break;
        case 6: // Longitude hemisphere
            sfr::gps::longitude[char_count] = term_buffer[0]; // TODO edge case
            break;
        case 10:
            sfr::gps::altitude->set_value(atof(term_buffer));
            break;
        }

        char_count = 0;
        term_count++;
        break;
    case '$':
        // new sentence
        term_count = 0;
        break;
    default:
        term_buffer[char_count] = c;
    }

    char_count++;
}

void GPSMonitor::execute()
{
    // Check to see if anything is available in the serial receive buffer
    while (ss.available() > 0) {
        if (encode(ss.read())) {

        }

        // // if valid, update the fields
        // if (check_GPGGA() == 1) {
        //     //parse_gps();

        //     // for testing purpose
        //     Serial.print("NEW MSG UPDATE!: ");
        //     print_dog();
        // }
    }
}