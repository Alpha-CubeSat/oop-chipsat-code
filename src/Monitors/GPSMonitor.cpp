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

void GPSMonitor::execute()
{
    if (!sfr::gps::on) {
        if (millis() - sfr::gps::boot_time > constants::gps::boot_time) {
            init();
        } else {
            return;
        }
    }

    int max_buffer_read = 0;

    while (ss.available() > 0 && max_buffer_read < _SS_MAX_RX_BUFF) {
        gps.encode(ss.read());
    }
    // while (*gpsStream) {
    //     gps.encode(*gpsStream++);
    // }

    if (gps.location.isUpdated() && gps.location.isValid()) {
        sfr::gps::latitude = gps.location.lat();
        sfr::gps::longitude = gps.location.lng();
    }

    if (gps.altitude.isUpdated() && gps.altitude.isValid()) {
        sfr::gps::altitude = gps.altitude.meters();
    }

    if (gps.time.isUpdated() && gps.time.isValid()) {
        sfr::gps::utc_time = gps.time.value();
    }

#ifdef VERBOSE
    Serial.print(F("Chars="));
    Serial.print(gps.charsProcessed());
    Serial.print(F(" Sentences-with-Fix="));
    Serial.print(gps.sentencesWithFix());
    Serial.print(F(" Failed-checksum="));
    Serial.print(gps.failedChecksum());
    Serial.print(F(" Passed-checksum="));
    Serial.println(gps.passedChecksum());
#endif
}