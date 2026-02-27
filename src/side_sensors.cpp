#include "config.h"
#include "side_sensors.h"
#include <Wire.h>


SideSensors* SideSensors::s_instance = nullptr;

SideSensors::SideSensors() 
    : m_leftSensor(SIDE_LEFT_INT_PIN, SIDE_LEFT_XSHUT_PIN, SIDE_LEFT_ADDRESS),
    m_rightSensor(SIDE_RIGHT_INT_PIN, SIDE_RIGHT_XSHUT_PIN, SIDE_RIGHT_ADDRESS) {}

bool SideSensors::begin() {
    Wire1.begin(I2C_WIRE1_SDA, I2C_WIRE1_SCL);
    Wire1.setClock(I2C_WIRE1_CLOCK_FREQ);

    m_leftSensor.initHardware();
    m_rightSensor.initHardware();

    if (!m_leftSensor.begin(&Wire1, VL53L0X_SIGNAL_RATE_LIMIT, VL53L0X_VCSEL_PULSE_PERIOD_PRE, VL53L0X_VCSEL_PULSE_PERIOD_FINAL, VL53L0X_MEASUREMENT_TIMING_BUDGET)) { return false; }
    if (!m_rightSensor.begin(&Wire1, VL53L0X_SIGNAL_RATE_LIMIT, VL53L0X_VCSEL_PULSE_PERIOD_PRE, VL53L0X_VCSEL_PULSE_PERIOD_FINAL, VL53L0X_MEASUREMENT_TIMING_BUDGET)) { return false; }
    
    attachInterrupts();
    
    m_leftSensor.clearPendingInterrupt();
    m_rightSensor.clearPendingInterrupt();

    return true;
}

void SideSensors::attachInterrupts() {
    s_instance = this;
    ::attachInterrupt(digitalPinToInterrupt(m_leftSensor.getIntPin()), leftISR, FALLING);
    ::attachInterrupt(digitalPinToInterrupt(m_rightSensor.getIntPin()), rightISR, FALLING);
}

void IRAM_ATTR SideSensors::leftISR() {
    if (s_instance) {
        s_instance->m_leftSensor.dataReadyISR();
    }
}

void IRAM_ATTR SideSensors::rightISR() {
    if (s_instance) {
        s_instance->m_rightSensor.dataReadyISR();
    }
}

void SideSensors::recoverBus() {
    ::detachInterrupt(digitalPinToInterrupt(m_leftSensor.getIntPin()));
    ::detachInterrupt(digitalPinToInterrupt(m_rightSensor.getIntPin()));

    Wire1.end();
    
    pinMode(I2C_WIRE1_SDA, INPUT_PULLUP);
    pinMode(I2C_WIRE1_SCL, INPUT_PULLUP);
    delay(1);

    if (digitalRead(I2C_WIRE1_SDA) == LOW) {
        for (int i = 0; i < 9; i++) {
            pinMode(I2C_WIRE1_SCL, OUTPUT);
            digitalWrite(I2C_WIRE1_SCL, LOW);
            delayMicroseconds(5);
            
            pinMode(I2C_WIRE1_SCL, INPUT_PULLUP); 
            delayMicroseconds(5);
            
            if (digitalRead(I2C_WIRE1_SDA) == HIGH) {
                break;
            }
        }
    }

    pinMode(I2C_WIRE1_SDA, OUTPUT);
    digitalWrite(I2C_WIRE1_SDA, LOW);
    delayMicroseconds(5);
    pinMode(I2C_WIRE1_SCL, INPUT_PULLUP); 
    delayMicroseconds(5);
    pinMode(I2C_WIRE1_SDA, INPUT_PULLUP); 
    delay(1);

    begin();
}

void SideSensors::updateData(uint16_t& leftSensorPlaceToWrite, uint16_t& rightSensorPlaceToWrite) {
    if (digitalRead(I2C_WIRE1_SDA) == LOW) {
        static uint32_t lastRecoveryTime = 0;
        if (millis() - lastRecoveryTime > 500) { 
            recoverBus();
            lastRecoveryTime = millis();
        }
        return;
    }

    m_leftSensor.updateData(leftSensorPlaceToWrite);
    m_rightSensor.updateData(rightSensorPlaceToWrite);
}