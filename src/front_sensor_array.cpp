#include "pinout.h"
#include "front_sensor_array.h"
#include <Wire.h>


#define sensorDistanceMode VL53L1X::DistanceMode::Medium
#define sensorTimingBudget 20000u

FrontSensorArray::FrontSensorArray() 
    : m_leftSensor(VL53L1X_LEFT_INT_PIN, VL53L1X_LEFT_XSHUT_PIN, VL53L1X_LEFT_ADDRESS),
    m_centerSensor(VL53L1X_CENTER_INT_PIN, VL53L1X_CENTER_XSHUT_PIN, VL53L1X_CENTER_ADDRESS),
    m_rightSensor(VL53L1X_RIGHT_INT_PIN, VL53L1X_RIGHT_XSHUT_PIN, VL53L1X_RIGHT_ADDRESS) {}

bool FrontSensorArray::begin(uint32_t timingBudget) {
    Wire.begin(I2C_WIRE_SDA, I2C_WIRE_SCL);
    Wire.setClock(I2C_WIRE_CLOCK_FREQ);

    m_leftSensor.initHardware();
    m_centerSensor.initHardware();
    m_rightSensor.initHardware();

    
    if (!m_leftSensor.begin(&Wire, sensorDistanceMode, timingBudget)) {return false;}
    if (!m_centerSensor.begin(&Wire, sensorDistanceMode, timingBudget)) {return false;}
    if (!m_rightSensor.begin(&Wire, sensorDistanceMode, timingBudget)) {return false;}

    return true;
}

void FrontSensorArray::updateData(uint16_t& leftSensorPlaceToWrite, uint16_t& centerSensorPlaceToWrite, uint16_t& rightSensorPlaceToWrite) {
    m_leftSensor.updateData(leftSensorPlaceToWrite);
    m_centerSensor.updateData(centerSensorPlaceToWrite);
    m_rightSensor.updateData(rightSensorPlaceToWrite);
}