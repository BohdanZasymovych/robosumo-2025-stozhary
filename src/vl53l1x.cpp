#include "vl53l1x.h"
#include <climits>


SensorVL53L1X::SensorVL53L1X(uint8_t intPin, uint8_t xShutPin, uint8_t address)
    : m_intPin(intPin),
    m_xShutPin(xShutPin),
    m_address(address),
    m_dataReadyFlag(false) {}

uint8_t SensorVL53L1X::getIntPin() {
    return m_intPin;
}

void SensorVL53L1X::initHardware() {
    pinMode(m_xShutPin, OUTPUT);
    digitalWrite(m_xShutPin, LOW);
    pinMode(m_intPin, INPUT_PULLUP);
}

bool SensorVL53L1X::begin(TwoWire* i2cBus, VL53L1X::DistanceMode distanceMode, uint32_t timingBudgetUs) {
    digitalWrite(m_xShutPin, HIGH);
    delay(10);

    m_sensor.setBus(i2cBus);
    m_sensor.setTimeout(500);

    if (!m_sensor.init()) {return false;}

    m_sensor.setAddress(m_address);
    m_sensor.setDistanceMode(distanceMode);
    m_sensor.setMeasurementTimingBudget(timingBudgetUs);
    m_sensor.startContinuous(20);

    return true;
}

void SensorVL53L1X::updateData(uint16_t& placeToWrite) {
    if (m_dataReadyFlag.exchange(false, std::memory_order_acquire)) {
        m_sensor.read(false);
        placeToWrite = (m_sensor.ranging_data.range_status != 0) ? UINT16_MAX : m_sensor.ranging_data.range_mm;
        // Serial.printf("range_mm: %d, range_status: %d, peak_signal_count_rate_MCPS: %f, ambient_count_rate_MCPS: %f\n", m_sensor.ranging_data.range_mm, m_sensor.ranging_data.range_status, m_sensor.ranging_data.peak_signal_count_rate_MCPS, m_sensor.ranging_data.ambient_count_rate_MCPS);
    }
}

void SensorVL53L1X::clearPendingInterrupt() {
    if (m_sensor.dataReady()) {
        m_sensor.read(false);
    }

    m_dataReadyFlag.store(false, std::memory_order_release);
}


void IRAM_ATTR SensorVL53L1X::dataReadyISR() {
    m_dataReadyFlag.store(true, std::memory_order_release);
}