#ifndef VL53L1X_H
#define VL53L1X_H


#include <VL53L1X.h>
#include <atomic>


class SensorVL53L1X {
private:
    const uint8_t m_intPin;
    const uint8_t m_xShutPin;
    const uint8_t m_address;

    std::atomic<bool> m_dataReadyFlag;

    VL53L1X m_sensor;

public:
    SensorVL53L1X(uint8_t intPin, uint8_t xShutPin, uint8_t address);
    uint8_t getIntPin();
    void initHardware();
    bool begin(TwoWire* i2cBus, VL53L1X::DistanceMode distanceMode, uint32_t timingBudget);
    void updateData(uint16_t& placeToWrite);
    void clearPendingInterrupt();

    void IRAM_ATTR dataReadyISR();
};


#endif // VL53L1X_H