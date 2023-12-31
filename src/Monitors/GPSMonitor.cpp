#include <Arduino.h>
#include <SoftwareSerial.h>

#include "GPSMonitor.hpp"

GPSMonitor::GPSMonitor()
{
    uint8_t SetNMEA[] = {
        0xA0, 0xA1, 0x00, 0x03, 0x09, 0x01, 0x01, 0x09, 0x0D, 0x0A};

    uint8_t SetPosRate[] = {
        0xA0, 0xA1, 0x00, 0x03, 0x0E, 0x01, 0x01, 0x0E, 0x0D, 0x0A};

    mySerial.begin(115200);
    // delay(1000);
    mySerial.write((uint8_t *)&SetNMEA, sizeof(SetNMEA));
    // delay(500);
    // mySerial.write((uint8_t *)&SetNMEA, sizeof(SetNMEA));
    // delay(500);
    // mySerial.write((uint8_t *)&SetNMEA, sizeof(SetNMEA));
    // delay(1000);

    sfr::gps::latitude->set_valid();
    sfr::gps::longitude->set_valid();
    sfr::gps::altitude->set_valid();
    sfr::gps::utc_h->set_valid();
    sfr::gps::utc_m->set_valid();
    sfr::gps::utc_s->set_valid();
}

void GPSMonitor::print_dog()
{
    for (int i = 0; i < (constants::gps::dog_size); i++) {
        Serial.print(sfr::gps::watch_dog[i]);
    }

    Serial.print("\n");
}

// GPGGA,031434.000,4226.63801,N,07630.10c90,W,0,00,0.0,105.7,M,0
void GPSMonitor::buffer_shift(char new_char)
{
    char temp_dog[constants::gps::dog_size];
    for (int i = 0; i < constants::gps::dog_size - 1; i++) {
        temp_dog[i] = sfr::gps::watch_dog[i + 1];
    }
    temp_dog[constants::gps::dog_size - 1] = new_char;

    // full copy
    for (int i = 0; i < constants::gps::dog_size; i++) {
        sfr::gps::watch_dog[i] = temp_dog[i];
    }

    sfr::gps::watch_dog[constants::gps::dog_size] = '\0';
}

int GPSMonitor::check_GPGGA()
{
    char c0 = sfr::gps::watch_dog[0];
    char c1 = sfr::gps::watch_dog[1];
    char c2 = sfr::gps::watch_dog[2];
    char c3 = sfr::gps::watch_dog[3];
    char c4 = sfr::gps::watch_dog[4];

    if (c0 == 'G' && c1 == 'P' && c2 == 'G' && c3 == 'G' && c4 == 'A') {
        return 1;
    } else {
        return 0;
    }
}

// precision = 0 , 1 , 2, for hour min sec
float GPSMonitor::token_to_time(char *token, int precision)
{
    // HHMMSS
    char hh[3];
    char mm[3];
    char ss_sss[6];

    for (int i = 0; i < 2; i++) {
        hh[i] = token[i];
        mm[i] = token[i + 2];
    }

    for (int i = 0; i < 5; i++) {
        ss_sss[i] = token[i + 4];
    }

    hh[2] = '\0';
    mm[2] = '\0';
    ss_sss[5] = '\0';

    switch (precision) {
    case 0: // return hour
        return atof(hh);
    case 1: // return min
        return atof(mm);
    case 2: // return sec
        return atof(ss_sss);
    }
}

// convert token for either Lat or Long to degree
float GPSMonitor::token_to_degree(char *token, bool is_lat)
{
    float d, m;

    if (is_lat) {
        char Lat_dd[2 + 1];
        char Lat_mm_mmmmm[8 + 1];

        for (int i = 0; i < 2; i++) {
            Lat_dd[i] = token[i];
        }

        for (int i = 0; i < 8; i++) {
            Lat_mm_mmmmm[i] = token[2 + i];
        }

        Lat_dd[2] = '\0';
        Lat_mm_mmmmm[8] = '\0';

        d = atof(Lat_dd);
        m = atof(Lat_mm_mmmmm);

    } else {
        char Long_ddd[3 + 1];
        char Long_mm_mmmmm[8 + 1];

        for (int i = 0; i < 3; i++) {
            Long_ddd[i] = token[i];
        }

        for (int i = 0; i < 8; i++) {
            Long_mm_mmmmm[i] = token[3 + i];
        }

        Long_ddd[3] = '\0';
        Long_mm_mmmmm[8] = '\0';

        d = atof(Long_ddd);
        m = atof(Long_mm_mmmmm);
    }

    return d + m / 60.0;
}

void GPSMonitor::parse_gps()
{
    int token_number = 0;
    char *token = strtok(sfr::gps::watch_dog, ",");

    while (token != NULL) {
        Serial.print("TOKEN NUMBER: ");
        Serial.println(token_number);
        switch (token_number) {
        case 1: // UTC time
            sfr::gps::utc_h->set_value(token_to_time(token, 0));
            Serial.print("UTC_H: ");
            Serial.println(token_to_time(token, 0));
            sfr::gps::utc_m->set_value(token_to_time(token, 1));
            sfr::gps::utc_s->set_value(token_to_time(token, 2));
        case 2: // Latitude
            sfr::gps::latitude->set_value(token_to_degree(token, true));
            Serial.print("Latitude");
            Serial.println(token_to_degree(token, true));
            break;
        case 3: // Latitude N/S indicator
            if (token[0] == 'S') {
                float latitude;
                sfr::gps::latitude->set_value(-sfr::gps::latitude->get_value(&latitude));
                Serial.print("Latitude N/S");
                Serial.println(-sfr::gps::latitude->get_value(&latitude));
            }
            break;
        case 4: // Longitude
            sfr::gps::longitude->set_value(token_to_degree(token, false));
            Serial.print("Longitude");
            Serial.println(token_to_degree(token, false));
            break;
        case 5: // Longitude E/W indicator
            if (token[0] == 'W') {
                float longitude;
                sfr::gps::longitude->set_value(-sfr::gps::longitude->get_value(&longitude));
                Serial.print("Longitude E/W");
                Serial.println(-sfr::gps::longitude->get_value(&longitude));
            }
            break;
        case 9: // Altitude
            sfr::gps::altitude->set_value(atof(token));
            Serial.print("Altitude");
            Serial.println(atof(token));
            break;
        }
        token = strtok(NULL, ",");
        token_number++;
    }
}

void GPSMonitor::execute()
{
    // Check to see if anything is available in the serial receive buffer
    while (mySerial.available() > 0) {
        // Read the next available byte in the serial receive buffer
        char new_char = mySerial.read();
        buffer_shift(new_char);
        // if valid, update the fields
        if (check_GPGGA() == 1) {
            parse_gps();

            // for testing purpose
            Serial.print("NEW MSG UPDATE!: ");
            print_dog();
        }
    }
}