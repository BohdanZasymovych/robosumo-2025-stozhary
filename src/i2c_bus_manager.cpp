#include "i2c_bus_manager.h"

I2CBusManager::I2CBusManager(TwoWire* wire, int sdaPin, int sclPin, uint32_t clockFreq)
    : m_wire(wire), m_sdaPin(sdaPin), m_sclPin(sclPin), m_clockFreq(clockFreq) {
}

void I2CBusManager::begin() {
    m_wire->begin(m_sdaPin, m_sclPin);
    m_wire->setClock(m_clockFreq);
    m_wire->setTimeOut(10); 
}

bool I2CBusManager::isBusStuck() {
    return digitalRead(m_sdaPin) == LOW;
}

void I2CBusManager::recoverBus() {
    m_wire->end();
    
    pinMode(m_sdaPin, INPUT_PULLUP);
    pinMode(m_sclPin, INPUT_PULLUP);
    delay(1);

    if (digitalRead(m_sdaPin) == LOW) {
        for (int i = 0; i < 9; i++) {
            pinMode(m_sclPin, OUTPUT);
            digitalWrite(m_sclPin, LOW);
            delayMicroseconds(5);
            
            pinMode(m_sclPin, INPUT_PULLUP); 
            delayMicroseconds(5);
            
            if (digitalRead(m_sdaPin) == HIGH) {
                break;
            }
        }
    }

    pinMode(m_sdaPin, OUTPUT);
    digitalWrite(m_sdaPin, LOW);
    delayMicroseconds(5);
    
    pinMode(m_sclPin, INPUT_PULLUP); 
    delayMicroseconds(5);
    
    pinMode(m_sdaPin, INPUT_PULLUP); 
    delay(1);

    begin();
}

TwoWire* I2CBusManager::getWire() {
    return m_wire;
}