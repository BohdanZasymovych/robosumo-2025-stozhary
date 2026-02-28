#ifndef LADLE_DISTANCE_SENSOR_H
#define LADLE_DISTANCE_SENSOR_H

#include "sensor_vl53l1x.h"
#include "i2c_bus_manager.h"

class LadleDistanceSensor {
private:
    SensorVL53L1X m_sensor;
    
    I2CBusManager* m_busManager;
    
    static LadleDistanceSensor* s_instance;

    static void IRAM_ATTR sensorISR();
    void attachInterrupts();
    void recoverBus();

public:
    LadleDistanceSensor();
    
    bool begin(I2CBusManager* busManager); 
    
    void updateData(uint16_t& placeToWrite);
};

#endif // LADLE_DISTANCE_SENSOR_H