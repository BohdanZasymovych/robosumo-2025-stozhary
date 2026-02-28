#include "config.h"
#include "side_sensors.h"
#include <Wire.h>

SideSensors* SideSensors::s_instance = nullptr;

SideSensors::SideSensors() 
    : m_leftSensor(VL53L0X_LEFT_INT_PIN, VL53L0X_LEFT_XSHUT_PIN, VL53L0X_LEFT_ADDRESS),
    m_rightSensor(VL53L0X_RIGHT_INT_PIN, VL53L0X_RIGHT_XSHUT_PIN, VL53L0X_RIGHT_ADDRESS) {}

bool SideSensors::begin(I2CBusManager* busManager) {
    m_busManager = busManager;

    m_leftSensor.initHardware();
    m_rightSensor.initHardware();

    if (!m_leftSensor.begin(m_busManager->getWire(), VL53L0X_SIGNAL_RATE_LIMIT, VL53L0X_VCSEL_PULSE_PERIOD_PRE, VL53L0X_VCSEL_PULSE_PERIOD_FINAL, VL53L0X_MEASUREMENT_TIMING_BUDGET)) { return false; }
    if (!m_rightSensor.begin(m_busManager->getWire(), VL53L0X_SIGNAL_RATE_LIMIT, VL53L0X_VCSEL_PULSE_PERIOD_PRE, VL53L0X_VCSEL_PULSE_PERIOD_FINAL, VL53L0X_MEASUREMENT_TIMING_BUDGET)) { return false; }
    
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

    m_busManager->recoverBus();

    begin(m_busManager);
}

void SideSensors::updateData(uint16_t& leftSensorPlaceToWrite, uint16_t& rightSensorPlaceToWrite) {
    if (m_busManager->isBusStuck()) {
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