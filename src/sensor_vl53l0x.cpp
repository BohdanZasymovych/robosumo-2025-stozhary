#include "sensor_vl53l0x.h"


SensorVL53L0X::SensorVL53L0X(uint8_t intPin, uint8_t xShutPin, uint8_t address)
    : m_intPin(intPin),
    m_xShutPin(xShutPin),
    m_address(address),
    m_dataReadyFlag(false) {}

uint8_t SensorVL53L0X::getIntPin() {
    return m_intPin;
}

void SensorVL53L0X::initHardware() {
    pinMode(m_xShutPin, OUTPUT);
    digitalWrite(m_xShutPin, LOW);
    pinMode(m_intPin, INPUT_PULLUP);
}

bool SensorVL53L0X::begin(TwoWire* i2cBus, float signalRateLimit, uint8_t vcselPulsePeriodPre, uint8_t vcselPulsePeriodFinal, uint32_t measurementTimingBudget) {
    digitalWrite(m_xShutPin, HIGH);
    delay(10);

    m_sensor.setBus(i2cBus);
    m_sensor.setTimeout(500);

    if (!m_sensor.init()) { return false; }

    m_sensor.setAddress(m_address);

    if (!m_sensor.setSignalRateLimit(signalRateLimit)) { return false; }
    if (!m_sensor.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, vcselPulsePeriodPre)) { return false; }
    if (!m_sensor.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, vcselPulsePeriodFinal)) { return false; }
    if (!m_sensor.setMeasurementTimingBudget(measurementTimingBudget)) { return false; }

    m_sensor.startContinuous();

    return true;
}

void SensorVL53L0X::updateData(uint16_t& placeToWrite) {
    if (m_dataReadyFlag.exchange(false, std::memory_order_acquire)) {
        placeToWrite = m_sensor.readRangeContinuousMillimeters();
    }
}

void SensorVL53L0X::clearPendingInterrupt() {
    m_sensor.readRangeContinuousMillimeters();
    m_dataReadyFlag.store(false, std::memory_order_release);
}

void IRAM_ATTR SensorVL53L0X::dataReadyISR() {
    m_dataReadyFlag.store(true, std::memory_order_release);
}