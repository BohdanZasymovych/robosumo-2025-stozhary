#include "config.h"
#include "front_sensor_array.h"
#include <Wire.h>

FrontSensorArray* FrontSensorArray::s_instance = nullptr;

FrontSensorArray::FrontSensorArray() 
    : m_leftSensor(VL53L0X_FRONT_LEFT_INT_PIN, VL53L0X_FRONT_LEFT_XSHUT_PIN, VL53L0X_FRONT_LEFT_ADDRESS),
    m_centerSensor(VL53L1X_FRONT_CENTER_INT_PIN, VL53L1X_FRONT_CENTER_XSHUT_PIN, VL53L1X_FRONT_CENTER_ADDRESS),
    m_rightSensor(VL53L0X_FRONT_RIGHT_INT_PIN, VL53L0X_FRONT_RIGHT_XSHUT_PIN, VL53L0X_FRONT_RIGHT_ADDRESS) {}

bool FrontSensorArray::begin() {
    Wire.begin(I2C_WIRE_SDA, I2C_WIRE_SCL);
    Wire.setClock(I2C_WIRE_CLOCK_FREQ);
    
    Wire.setTimeOut(10); 

    m_leftSensor.initHardware();
    m_centerSensor.initHardware();
    m_rightSensor.initHardware();

    if (!m_leftSensor.begin(&Wire, VL53L0X_SIGNAL_RATE_LIMIT, VL53L0X_VCSEL_PULSE_PERIOD_PRE, VL53L0X_VCSEL_PULSE_PERIOD_FINAL, VL53L0X_MEASUREMENT_TIMING_BUDGET)) { return false; }
    if (!m_centerSensor.begin(&Wire, VL53L1X_SENSOR_DISTANCE_MODE, VL53L1X_TIMING_BUDGET)) {return false;}
    if (!m_rightSensor.begin(&Wire, VL53L0X_SIGNAL_RATE_LIMIT, VL53L0X_VCSEL_PULSE_PERIOD_PRE, VL53L0X_VCSEL_PULSE_PERIOD_FINAL, VL53L0X_MEASUREMENT_TIMING_BUDGET)) { return false; }
    
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

    Wire.end();
    
    pinMode(I2C_WIRE_SDA, INPUT_PULLUP);
    pinMode(I2C_WIRE_SCL, INPUT_PULLUP);
    delay(1);

    if (digitalRead(I2C_WIRE_SDA) == LOW) {
        for (int i = 0; i < 9; i++) {
            pinMode(I2C_WIRE_SCL, OUTPUT);
            digitalWrite(I2C_WIRE_SCL, LOW);
            delayMicroseconds(5);
            
            pinMode(I2C_WIRE_SCL, INPUT_PULLUP); 
            delayMicroseconds(5);
            
            if (digitalRead(I2C_WIRE_SDA) == HIGH) {
                break;
            }
        }
    }

    pinMode(I2C_WIRE_SDA, OUTPUT);
    digitalWrite(I2C_WIRE_SDA, LOW);
    delayMicroseconds(5);
    pinMode(I2C_WIRE_SCL, INPUT_PULLUP); 
    delayMicroseconds(5);
    pinMode(I2C_WIRE_SDA, INPUT_PULLUP); 
    delay(1);

    begin();
}

void FrontSensorArray::updateData(uint16_t& leftSensorPlaceToWrite, uint16_t& centerSensorPlaceToWrite, uint16_t& rightSensorPlaceToWrite) {
    if (digitalRead(I2C_WIRE_SDA) == LOW) {
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