#include "config.h"
#include "ladle_distance_sensor.h"
#include "i2c_bus_manager.h"

LadleDistanceSensor* LadleDistanceSensor::s_instance = nullptr;

LadleDistanceSensor::LadleDistanceSensor() 
    : m_sensor(VL53L1X_LADLE_INT_PIN, VL53L1X_LADLE_XSHUT_PIN, VL53L1X_LADLE_ADDRESS) {}

bool LadleDistanceSensor::begin(I2CBusManager* busManager) {
    m_busManager = busManager;

    m_sensor.initHardware();

    if (!m_sensor.begin(m_busManager->getWire(), VL53L1X_LADLE_SENSOR_DISTANCE_MODE, VL53L1X_LADLE_TIMING_BUDGET)) {return false;}
    
    attachInterrupts();
    
    m_sensor.clearPendingInterrupt();

    return true;
}

void LadleDistanceSensor::attachInterrupts() {
    s_instance = this;
    ::attachInterrupt(digitalPinToInterrupt(m_sensor.getIntPin()), sensorISR, FALLING);
}

void IRAM_ATTR LadleDistanceSensor::sensorISR() {
    if (s_instance) {
        s_instance->m_sensor.dataReadyISR();
    }
}

void LadleDistanceSensor::recoverBus() {
    ::detachInterrupt(digitalPinToInterrupt(m_sensor.getIntPin()));

    m_busManager->recoverBus();

    begin(m_busManager);
}

void LadleDistanceSensor::updateData(uint16_t& placeToWrite) {
    if (m_busManager->isBusStuck()) {
        static uint32_t lastRecoveryTime = 0;
        if (millis() - lastRecoveryTime > 500) { 
            recoverBus();
            lastRecoveryTime = millis();
        }
        return;
    }

    m_sensor.updateData(placeToWrite);
}