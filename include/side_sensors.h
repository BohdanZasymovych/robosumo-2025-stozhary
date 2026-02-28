#ifndef SIDE_SENSORS_H
#define SIDE_SENSORS_H


#include "sensor_vl53l0x.h"


class SideSensors {
private:
    SensorVL53L0X m_leftSensor;
    SensorVL53L0X m_rightSensor;
    SensorVL53L0X m_ladleSensor;
    
    static SideSensors* s_instance;
    
    static void IRAM_ATTR leftISR();
    static void IRAM_ATTR rightISR();
    static void IRAM_ATTR ladleISR();
    void attachInterrupts();
    void recoverBus();

public:
    SideSensors();
    bool begin();
    void updateData(uint16_t& leftSensorPlaceToWrite,
                    uint16_t& rightSensorPlaceToWrite,
                    uint16_t& ladleSensorPlaceToWrite);
};


#endif // SIDE_SENSORS_H