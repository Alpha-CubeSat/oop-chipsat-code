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
    sfr::gps::latitude->set_valid();
    sfr::gps::longitude->set_valid();
    sfr::gps::utc_time->set_valid();
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
bool GPSMonitor::encode(char c)
{
    switch (c) {
    case ',':
        // clear term buffer and udpate sfr if needed
        Serial.print("Term: ");

        for (int i = 0; i < char_count; i++) {
            Serial.print(term_buffer[i]);
        }
        Serial.println();
        if (!valid_msg)
            break;
        switch (term_count) {
        case 0:
            if (!check_GPGGA()) {
                valid_msg = false;
                Serial.println("INVALID MESSAGE");
            }
            break;
        case 1: // UTC
            Serial.println("UTC");

            char temp_buf[11];
            strncpy(temp_buf, term_buffer, 10);
            temp_buf[10] = '\0';

            sfr::gps::utc_time->set_value(atof(temp_buf));

            break;
        case 2: // Latitude value
        {
            Serial.println("Latitude val");
            char temp_buf[11];

            strncpy(temp_buf, term_buffer, 10);
            temp_buf[10] = '\0';

            Serial.print("Valid: ");
            Serial.println(atof(temp_buf));

            sfr::gps::latitude->set_value(atof(temp_buf));
            break;
        }
        case 3: // Latitude hemisphere
        {
            if (term_buffer[0] == 'S') {
                float latitude;
                sfr::gps::latitude->get_value(&latitude);
                sfr::gps::latitude->set_value(-latitude);
                Serial.println(latitude);
            }
            break;
        }
        case 4: // Longitude value
        {
            Serial.println("Longitude val");
            char temp_buf[12];
            strncpy(temp_buf, term_buffer, 11);
            temp_buf[11] = '\0';

            Serial.print("Valid: ");
            Serial.println(atof(temp_buf));

            sfr::gps::longitude->set_value(atof(temp_buf));
            break;
        }
        case 5: // Longitude hemisphere
        {
            if (term_buffer[0] == 'W') {
                float longitude;
                sfr::gps::longitude->get_value(&longitude);
                sfr::gps::longitude->set_value(-longitude);
                Serial.println(longitude);
            }
            break;
        }
        case 9: // Altitude
        {
            Serial.println("Altitude");

            char temp_buf[char_count];
            strncpy(temp_buf, term_buffer, char_count - 1);
            temp_buf[char_count - 1] = '\0';

            sfr::gps::altitude->set_value(atof(temp_buf));
        }
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
    case '\n':
        term_count = 0;
        char_count = 0;
        // End of sentence
        return true;
    default:
        term_buffer[char_count] = c;
        char_count++;
        break;
    }

    return false;
}

void GPSMonitor::execute()
{
    // Check to see if anything is available in the serial receive buffer
    // while (*gpsStream) {
    //     if (encode(*gpsStream++)) {

    //     }
    // }

    while (ss.available() > 0) {
        if (encode(ss.read())) {
        }
    }
}