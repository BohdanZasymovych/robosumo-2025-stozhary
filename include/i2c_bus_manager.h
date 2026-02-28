#ifndef I2C_BUS_MANAGER_H
#define I2C_BUS_MANAGER_H

#include <Arduino.h>
#include <Wire.h>

class I2CBusManager {
private:
    TwoWire* m_wire;
    int m_sdaPin;
    int m_sclPin;
    uint32_t m_clockFreq;

public:
    I2CBusManager(TwoWire* wire, int sdaPin, int sclPin, uint32_t clockFreq = 100000);

    void begin();
    bool isBusStuck();
    void recoverBus();
    TwoWire* getWire();
};

#endif // I2C_BUS_MANAGER_H