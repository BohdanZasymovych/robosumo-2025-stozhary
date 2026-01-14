#ifndef FRONT_SENSOR_ARRAY_H
#define FRONT_SENSOR_ARRAY_H


#include "vl53l1x.h"


class FrontSensorArray {
private:
    SensorVL53L1X m_leftSensor;
    SensorVL53L1X m_centerSensor;
    SensorVL53L1X m_rightSensor;
    
public:
    FrontSensorArray();
    bool begin(uint32_t timingBudget);
    void updateData(uint16_t& leftSensorPlaceToWrite,
                    uint16_t& centerSensorPlaceToWrite,
                    uint16_t& rightSensorPlaceToWrite);
};


#endif // FRONT_SENSOR_ARRAY_H