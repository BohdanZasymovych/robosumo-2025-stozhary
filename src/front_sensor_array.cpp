#include "config.h"
#include "front_sensor_array.h"
#include "i2c_bus_manager.h"

FrontSensorArray* FrontSensorArray::s_instance = nullptr;

FrontSensorArray::FrontSensorArray() 
    : m_leftSensor(VL53L0X_FRONT_LEFT_INT_PIN, VL53L0X_FRONT_LEFT_XSHUT_PIN, VL53L0X_FRONT_LEFT_ADDRESS),
    m_centerSensor(VL53L1X_FRONT_CENTER_INT_PIN, VL53L1X_FRONT_CENTER_XSHUT_PIN, VL53L1X_FRONT_CENTER_ADDRESS),
    m_rightSensor(VL53L0X_FRONT_RIGHT_INT_PIN, VL53L0X_FRONT_RIGHT_XSHUT_PIN, VL53L0X_FRONT_RIGHT_ADDRESS) {}

bool FrontSensorArray::begin(I2CBusManager* busManager) {
    m_busManager = busManager;

    m_leftSensor.initHardware();
    m_centerSensor.initHardware();
    m_rightSensor.initHardware();

    if (!m_leftSensor.begin(m_busManager->getWire(), VL53L0X_SIGNAL_RATE_LIMIT, VL53L0X_VCSEL_PULSE_PERIOD_PRE, VL53L0X_VCSEL_PULSE_PERIOD_FINAL, VL53L0X_MEASUREMENT_TIMING_BUDGET)) { return false; }
    if (!m_centerSensor.begin(m_busManager->getWire(), VL53L1X_SENSOR_DISTANCE_MODE, VL53L1X_TIMING_BUDGET)) {return false;}
    if (!m_rightSensor.begin(m_busManager->getWire(), VL53L0X_SIGNAL_RATE_LIMIT, VL53L0X_VCSEL_PULSE_PERIOD_PRE, VL53L0X_VCSEL_PULSE_PERIOD_FINAL, VL53L0X_MEASUREMENT_TIMING_BUDGET)) { return false; }
    
    attachInterrupts();
    
    m_leftSensor.clearPendingInterrupt();
    m_centerSensor.clearPendingInterrupt();
    m_rightSensor.clearPendingInterrupt();

    return true;
}

void FrontSensorArray::attachInterrupts() {
    s_instance = this;
    ::attachInterrupt(digitalPinToInterrupt(m_leftSensor.getIntPin()), leftISR, FALLING);
    ::attachInterrupt(digitalPinToInterrupt(m_centerSensor.getIntPin()), centerISR, FALLING);
    ::attachInterrupt(digitalPinToInterrupt(m_rightSensor.getIntPin()), rightISR, FALLING);
}

void IRAM_ATTR FrontSensorArray::leftISR() {
    if (s_instance) {
        s_instance->m_leftSensor.dataReadyISR();
    }
}

void IRAM_ATTR FrontSensorArray::centerISR() {
    if (s_instance) {
        s_instance->m_centerSensor.dataReadyISR();
    }
}

void IRAM_ATTR FrontSensorArray::rightISR() {
    if (s_instance) {
        s_instance->m_rightSensor.dataReadyISR();
    }
}

void FrontSensorArray::recoverBus() {
    ::detachInterrupt(digitalPinToInterrupt(m_leftSensor.getIntPin()));
    ::detachInterrupt(digitalPinToInterrupt(m_centerSensor.getIntPin()));
    ::detachInterrupt(digitalPinToInterrupt(m_rightSensor.getIntPin()));

    m_busManager->recoverBus();

    begin(m_busManager);
}

void FrontSensorArray::updateData(uint16_t& leftSensorPlaceToWrite, uint16_t& centerSensorPlaceToWrite, uint16_t& rightSensorPlaceToWrite) {
    if (m_busManager->isBusStuck()) {
        static uint32_t lastRecoveryTime = 0;
        if (millis() - lastRecoveryTime > 500) { 
            recoverBus();
            lastRecoveryTime = millis();
        }
        return;
    }

    m_leftSensor.updateData(leftSensorPlaceToWrite);
    m_centerSensor.updateData(centerSensorPlaceToWrite);
    m_rightSensor.updateData(rightSensorPlaceToWrite);
}