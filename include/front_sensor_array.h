#ifndef FRONT_SENSOR_ARRAY_H
#define FRONT_SENSOR_ARRAY_H


#include "vl53l0x.h"
#include "vl53l1x.h"


class FrontSensorArray {
private:
    SensorVL53L0X m_leftSensor;
    SensorVL53L1X m_centerSensor;
    SensorVL53L0X m_rightSensor;
    
    static FrontSensorArray* s_instance;
    
    static void IRAM_ATTR leftISR();
    static void IRAM_ATTR centerISR();
    static void IRAM_ATTR rightISR();
    void attachInterrupts();

public:
    FrontSensorArray();
    bool begin();
    void updateData(uint16_t& leftSensorPlaceToWrite,
                    uint16_t& centerSensorPlaceToWrite,
                    uint16_t& rightSensorPlaceToWrite);
};


#endif // FRONT_SENSOR_ARRAY_H