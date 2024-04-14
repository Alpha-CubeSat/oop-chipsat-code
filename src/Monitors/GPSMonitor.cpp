#include <Arduino.h>
#include <SoftwareSerial.h>
#include <string.h>

#include "GPSMonitor.hpp"

GPSMonitor::GPSMonitor()
{
}

void GPSMonitor::init()
{
    digitalWrite(constants::gps::reset_pin, LOW);
    sfr::gps::on = true;

    // writes NMEA output message type to SRAM and Flash
    uint8_t SetNMEA[] = {
        0xA0, 0xA1, 0x00, 0x03, 0x09, 0x01, 0x01, 0x09, 0x0D, 0x0A};

    ss.begin(115200);
    ss.write((uint8_t *)&SetNMEA, sizeof(SetNMEA));
}

bool GPSMonitor::check_GPGGA()
{
    if (term_buffer[0] == 'G' && term_buffer[1] == 'P' && term_buffer[2] == 'G' && term_buffer[3] == 'G' && term_buffer[4] == 'A') {
        return true;
    } else {
        return false;
    }
}

bool GPSMonitor::encode(char c)
{
    if (c == '$') {
        // new sentence
        char_count = 0;
        term_count = 0;
        sfr::gps::valid_msg = true;
        Serial.println(F("NEW GPS SENTENCE"));
    } else if (sfr::gps::valid_msg) {

        // Serial.print("char: ");
        // Serial.println(c);
        // Serial.print("char_count: ");
        // Serial.println(char_count);
        if (char_count > constants::gps::buffer_size) {
            sfr::gps::valid_msg = false;
            Serial.println("INVALID GPS MESSAGE");
        }
        if (c == ',') {
            // clear term buffer and udpate sfr if needed
            // Serial.print("Term: ");
            // for (int i = 0; i < char_count; i++) {
            //     Serial.print(term_buffer[i]);
            // }
            // Serial.println();

            switch (term_count) {
            case 0:
                if (!check_GPGGA()) {
                    sfr::gps::valid_msg = false;
#ifdef VERBOSE
                    Serial.println("INVALID GPS MESSAGE");
#endif
                }
                break;
            case 1: // UTC
                Serial.println("UTC");

                char temp_buf[11];
                strncpy(temp_buf, term_buffer, 10);
                temp_buf[10] = '\0';
                sfr::gps::utc_time = atof(temp_buf);
                Serial.println(sfr::gps::utc_time);

                break;
            case 2: // Latitude value
            {
                Serial.println("Latitude");

                char dd_buf[3] = {term_buffer[0], term_buffer[1], '\0'};
                char mm_buf[3] = {term_buffer[2], term_buffer[3], '\0'};
                // char dd_buf[3];
                // char mm_buf[3];
                // strncpy(dd_buf, term_buffer, 2);
                // strncpy(mm_buf, term_buffer + 2, 2);
                // dd_buf[2] = '\0';
                // mm_buf[2] = '\0';

                sfr::gps::latitude = atoi(dd_buf) + atoi(mm_buf) / 60.0;
                Serial.println(sfr::gps::latitude);

                break;
            }
            case 3: // Latitude hemisphere
            {
                if (term_buffer[0] == 'S') {
                    sfr::gps::latitude = -sfr::gps::latitude;
                    Serial.println(sfr::gps::latitude);
                }
                break;
            }
            case 4: // Longitude value
            {
                Serial.println("Longitude");

                char ddd_buf[4] = {term_buffer[0], term_buffer[1], term_buffer[2], '\0'};
                char mm_buf[3] = {term_buffer[3], term_buffer[4], '\0'};

                sfr::gps::longitude = atoi(ddd_buf) + atoi(mm_buf) / 60.0;
                Serial.println(sfr::gps::longitude);

                break;
            }
            case 5: // Longitude hemisphere
            {
                if (term_buffer[0] == 'W') {
                    sfr::gps::longitude = -sfr::gps::longitude;
                    Serial.println(sfr::gps::longitude);
                }
                break;
            }
            case 9: // Altitude
            {
                Serial.println("Altitude");

                char temp_buf[char_count];
                strncpy(temp_buf, term_buffer, char_count - 1);
                temp_buf[char_count - 1] = '\0';

                sfr::gps::altitude = atof(temp_buf);
                Serial.println(sfr::gps::altitude);

                break;
            }
            }

            char_count = 0;
            term_count++;
        } else if (c == '\n') {
            term_count = 0;
            char_count = 0;
            // End of sentence
            return true;
        } else {
            term_buffer[char_count] = c;
            char_count++;
        }
    }

    return false;
}

void GPSMonitor::execute()
{
    if (!sfr::gps::on) {
        if (millis() - sfr::gps::boot_time > constants::gps::boot_time) {
            init();
        } else {
            return;
        }
    }

    // TODO: verify max buffer size and look into GPS parsing lib
    int max_buffer_read = 0;

    // Check to see if anything is available in the serial receive buffer
    while (ss.available() > 0 && max_buffer_read < 64) {
        if (encode(ss.read())) {
#ifdef VERBOSE
            Serial.println(F("NEW GPS MESSAGE"));
#endif
        }
        max_buffer_read++;
    }

    // for testing
    // while (*gpsStream) {
    //     if (encode(*gpsStream++)) {
    //     }
    // }
}