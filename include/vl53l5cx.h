#ifndef VL53L5CX_H
#define VL53L5CX_H


#include <SparkFun_VL53L5CX_Library.h>
#include <array>
#include <optional>

#define IMAGE_RESOLUTION 16u
#define IMAGE_SIDE_SIZE 4u

using is_object_detected_matrix = std::array<std::array<bool, IMAGE_SIDE_SIZE>, IMAGE_SIDE_SIZE>;


class VL53L5CX {
private:
    uint8_t m_sclPin;
    uint8_t m_sdaPin;
    uint8_t m_intPin;

    SparkFun_VL53L5CX m_sensor;
    VL53L5CX_ResultsData m_data;
    volatile static bool m_DataReadyFlag;

    static void IRAM_ATTR m_dataReadyISR();
    
    void m_processReceivedData(is_object_detected_matrix& isObjectDetectedMatrix);

public:
    VL53L5CX(uint8_t sclPin, uint8_t sdaPin, uint8_t intPin);
    bool begin(uint8_t frequency);
    void getData(is_object_detected_matrix& isObjectDetectedMatrix);
    void printDistanceMatrix();
};


#endif