#ifndef SIDE_SENSORS_H
#define SIDE_SENSORS_H

#include "vl53l0x.h"
#include "i2c_bus_manager.h"

class SideSensors {
private:
    SensorVL53L0X m_leftSensor;
    SensorVL53L0X m_rightSensor;
    
    I2CBusManager* m_busManager;
    
    static SideSensors* s_instance;
    
    static void IRAM_ATTR leftISR();
    static void IRAM_ATTR rightISR();
    static void IRAM_ATTR ladleISR();
    void attachInterrupts();
    void recoverBus();

public:
    SideSensors();
    bool begin(I2CBusManager* busManager);
    void updateData(uint16_t& leftSensorPlaceToWrite,
                    uint16_t& rightSensorPlaceToWrite,
                    uint16_t& ladleSensorPlaceToWrite);
};

#endif // SIDE_SENSORS_H