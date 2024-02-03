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

bool GPSMonitor::check_GPGGA()
{
    if (term_buffer[0] == 'G' && term_buffer[1] == 'P' && term_buffer[2] == 'G' && term_buffer[3] == 'G' && term_buffer[4] == 'A') {
        return true;
    } else {
        return false;
    }
}

// return true at end of term
bool GPSMonitor::encode(char c) {

    // Serial.print("First char of term_buffer: ");
    // Serial.println(term_buffer[0]);
    
    switch (c) {
    case ',':
        // clear term buffer and udpate sfr if needed
        // Serial.print("Term: ");

        // for (int i = 0; i < char_count; i++) {
        //     Serial.print(term_buffer[i]);
        // }
        // Serial.println();
        if (!valid_msg) break;
        switch (term_count) {
        case 0:
            if (!check_GPGGA()) {
                valid_msg = false;
                Serial.println("INVALID MESSAGE");
            }
            break;
        case 1: // UTC
            // Serial.println("UTC");
            memcpy(&sfr::gps::utc_time, &term_buffer, 10);
            break;
        case 2: // Latitude value
            // Serial.println("Latitude val");
            memcpy(&sfr::gps::latitude, &term_buffer, char_count);
            break;
        case 3: // Latitude hemisphere
            // Serial.println("Latitude hem");
            if (term_buffer[0] == 'S') {

            }
            //sfr::gps::latitude[char_count] = term_buffer[0];
            break;
        case 4: // Longitude value
            // Serial.println("Longitude val");
            memcpy(&sfr::gps::longitude, &term_buffer, char_count);
            break;
        case 5: // Longitude hemisphere
            // Serial.println("Longitude hem");
            sfr::gps::longitude[char_count] = term_buffer[0];
            break;
        case 9: // Altitude
            // Serial.println("Altitude");
            sfr::gps::altitude->set_value(atof(term_buffer));
            
            term_count = 0;
            char_count = 0;
            // End of meaningful sentence
            return true;
        }

        char_count = 0;
        term_count++;
        break;
    case '$':
        // new sentence
        char_count = 0;
        term_count = 0;
        valid_msg = true;
        break;
    default:
        term_buffer[char_count] = c;
        char_count++;
        break;
    }

    
}

void GPSMonitor::execute()
{
    // Check to see if anything is available in the serial receive buffer
    while (ss.available() > 0) {
        //char c = ss.read();
        //Serial.println(c);
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