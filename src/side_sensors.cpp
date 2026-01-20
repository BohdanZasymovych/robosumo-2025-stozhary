#include "pinout.h"
#include "side_sensors.h"
#include <Wire.h>


#define SIGNAL_RATE_LIMIT 0.25
#define VCSEL_PULSE_PERIOD_PRE 14u
#define VCSEL_PULSE_PERIOD_FINAL 10u
#define MEASUREMENT_TIMING_BUDGET 20000u

SideSensors* SideSensors::s_instance = nullptr;

SideSensors::SideSensors() 
    : m_leftSensor(SIDE_LEFT_INT_PIN, SIDE_LEFT_XSHUT_PIN, SIDE_LEFT_ADDRESS),
    m_rightSensor(SIDE_RIGHT_INT_PIN, SIDE_RIGHT_XSHUT_PIN, SIDE_RIGHT_ADDRESS) {}

bool SideSensors::begin() {
    Wire1.begin(I2C_WIRE1_SDA, I2C_WIRE1_SCL);
    Wire1.setClock(I2C_WIRE1_CLOCK_FREQ);

    m_leftSensor.initHardware();
    m_rightSensor.initHardware();

    if (!m_leftSensor.begin(&Wire1, SIGNAL_RATE_LIMIT, VCSEL_PULSE_PERIOD_PRE, VCSEL_PULSE_PERIOD_FINAL, MEASUREMENT_TIMING_BUDGET)) { return false; }
    if (!m_rightSensor.begin(&Wire1, SIGNAL_RATE_LIMIT, VCSEL_PULSE_PERIOD_PRE, VCSEL_PULSE_PERIOD_FINAL, MEASUREMENT_TIMING_BUDGET)) { return false; }
    
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

void SideSensors::updateData(uint16_t& leftSensorPlaceToWrite, uint16_t& rightSensorPlaceToWrite) {
    m_leftSensor.updateData(leftSensorPlaceToWrite);
    m_rightSensor.updateData(rightSensorPlaceToWrite);
}