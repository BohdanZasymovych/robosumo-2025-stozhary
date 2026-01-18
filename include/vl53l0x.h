#ifndef VL53L0X_H
#define VL53L0X_H


#include <VL53L0X.h>
#include <Wire.h>
#include <atomic>


class SensorVL53L0X {
private:
    const uint8_t m_intPin;
    const uint8_t m_xShutPin;
    const uint8_t m_address;

    std::atomic<bool> m_dataReadyFlag;

    VL53L0X m_sensor;

public:
    SensorVL53L0X(uint8_t intPin, uint8_t xShutPin, uint8_t address);
    uint8_t getIntPin();
    void initHardware();
    bool begin(TwoWire* i2cBus, float signalRateLimit, uint8_t vcselPulsePeriodPre, uint8_t vcselPulsePeriodFinal, uint32_t measurementTimingBudget);
    void updateData(uint16_t& placeToWrite);
    void clearPendingInterrupt();

    void IRAM_ATTR dataReadyISR();
};


#endif // VL53L0X_H