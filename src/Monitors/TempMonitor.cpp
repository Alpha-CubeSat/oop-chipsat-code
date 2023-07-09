#include "TempMonitor.hpp"

// seems wasteful to downlink temperature in 2 units

// Define the STS21 address for proper I2C selection
//  this should go in constants
#define addr 0x4A

TempMonitor::TempMonitor()
{
    // Init I2C communication as master
    Wire.begin();
    Serial.begin(9600);
}

void TempMonitor::execute()
{
    if (!initialized) {
        sfr::temp::temp_c->set_valid();
        sfr::temp::temp_f->set_valid();
        initialized = true;
    }

    unsigned int data[2];

    // Start I2C transmit
    Wire.beginTransmission(addr);
    // Select no hold master
    Wire.write(0xF3);
    Wire.endTransmission();
    delay(300);

    // Request 2 bytes of data
    Wire.requestFrom(addr, 2);

    if (Wire.available() == 2) {
        data[0] = Wire.read();
        data[1] = Wire.read();
    }

    // Convert the data
    int rawtmp = data[0] * 256 + data[1];
    int value = rawtmp & 0xFFFC;
    double temp_c = -46.85 + (175.72 * (value / 65536.0));
    double temp_f = temp_c * 1.8 + 32;

    // Output data to serial monitor
    Serial.print("Temperature in Celsius:  ");
    Serial.print(temp_c);
    Serial.println(" C");
    Serial.print("Temperature in Fahrenheit:  ");
    Serial.print(temp_f);
    Serial.println(" F");
    delay(300);
}
