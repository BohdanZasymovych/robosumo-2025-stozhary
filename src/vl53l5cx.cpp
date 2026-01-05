#include "vl53l5cx.h"
#include <Arduino.h>

#define DISTANCE_TRESHOLD_MM 300u


volatile bool VL53L5CX::m_DataReadyFlag = false;


void IRAM_ATTR VL53L5CX::m_dataReadyISR() {
    m_DataReadyFlag = true;
}

void VL53L5CX::m_processReceivedData(is_object_detected_matrix& isObjectDetectedMatrix) {
    for (uint8_t y = 0; y < IMAGE_SIDE_SIZE; ++y) {
        for (uint8_t x = 0; x < IMAGE_SIDE_SIZE; ++x) {
            uint16_t distance = m_data.distance_mm[y + x * IMAGE_SIDE_SIZE];
            isObjectDetectedMatrix[y][x] = (distance <= DISTANCE_TRESHOLD_MM) ? true : false;
        }
    }
}

VL53L5CX::VL53L5CX(uint8_t sclPin, uint8_t sdaPin, uint8_t intPin)
    : m_sclPin(sclPin), m_sdaPin(sdaPin), m_intPin(intPin) {}

bool VL53L5CX::begin(uint8_t frequency) {
    Wire.begin(m_sdaPin, m_sclPin);
    Wire.setClock(400000);

    if (m_sensor.begin() == false) {
        return false;
    }

    pinMode(m_intPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(m_intPin), VL53L5CX::m_dataReadyISR, FALLING);

    m_sensor.setResolution(IMAGE_RESOLUTION);
    m_sensor.setRangingFrequency(frequency);
    m_sensor.startRanging();

    return true;
}

void VL53L5CX::getData(is_object_detected_matrix& isObjectDetectedMatrix) {
    if (!m_DataReadyFlag) {
        return;
    }

    m_DataReadyFlag = false;
    m_sensor.getRangingData(&m_data);
    m_processReceivedData(isObjectDetectedMatrix);
}

void VL53L5CX::printDistanceMatrix() {
    Serial.println("-----------------------------------------");
    for (uint8_t y = 0; y < IMAGE_SIDE_SIZE; ++y) {
        for (uint8_t x = 0; x < IMAGE_SIDE_SIZE; ++x) {
            uint16_t distance = m_data.distance_mm[y + x * IMAGE_SIDE_SIZE];
            Serial.print(distance);
            Serial.print('\t');
        }
        Serial.println();
    }
}
