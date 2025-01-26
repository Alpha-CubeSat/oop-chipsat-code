#include "TempMonitor.hpp"

TempMonitor::TempMonitor()
{
    Wire.beginTransmission(constants::temperature::i2c_address);
    Wire.write(0xE7);
    Wire.endTransmission();

    Wire.requestFrom(constants::temperature::i2c_address, 1);

    if (Wire.available() == 1) {
        uint8_t user_register = Wire.read();
        user_register = user_register | 0x81;

        Wire.beginTransmission(constants::temperature::i2c_address);
        Wire.write(0xE6);
        Wire.write(user_register);
        Wire.endTransmission();
    }
}

void TempMonitor::execute()
{
    Wire.beginTransmission(constants::temperature::i2c_address);
    // Select no hold master
    Wire.write(0xF3);
    Wire.endTransmission();
    delay(15);

    // Request 2 bytes of data
    Wire.requestFrom(constants::temperature::i2c_address, 2);

    if (Wire.available() == 2) {
        data[0] = Wire.read();
        data[1] = Wire.read();
    }

    // Convert the data
    uint32_t raw_temp = (data[0] * 256 + data[1]) & 0xFFFC;
    sfr::temperature::temp_c = -46.85 + (175.72 * (raw_temp / 65536.0));
}
