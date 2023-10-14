#include "TempMonitor.hpp"

TempMonitor::TempMonitor()
{
    // Init I2C communication as master
    Wire.begin();
}

void TempMonitor::execute()
{
    if (!initialized) {
        sfr::temperature::temp_c->set_valid();
        sfr::temperature::temp_f->set_valid();
        initialized = true;
    }

    // Start I2C transmit
    Wire.beginTransmission(constants::temperature::i2c_address);
    // Select no hold master
    Wire.write(0xF3);
    Wire.endTransmission();
    delay(300);

    // Request 2 bytes of data
    Wire.requestFrom(constants::temperature::i2c_address, 2);

    if (Wire.available() == 2) {
        data[0] = Wire.read();
        data[1] = Wire.read();
    }

    // Convert the data
    uint32_t rawtmp = (data[0] * 256 + data[1]) & 0xFFFC;
    double temp_c = -46.85 + (175.72 * (rawtmp / 65536.0));
    double temp_f = temp_c * 1.8 + 32;

    sfr::temperature::temp_c->set_value(temp_c);
    sfr::temperature::temp_f->set_value(temp_f);
}
