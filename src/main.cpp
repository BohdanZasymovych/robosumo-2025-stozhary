#include <Arduino.h>
#include "motor.h"
#include "robot.h"
#include "front_sensor_array.h"

#define SENSOR_TIMING_BUDGET_US 20000u

uint16_t leftSensorDistance = 0;
uint16_t centerSensorDistance = 0;
uint16_t rightSensorDistance = 0;

FrontSensorArray frontSensorArray;

void setup() {
    Serial.begin(115200);
    
    if (!frontSensorArray.begin(SENSOR_TIMING_BUDGET_US)) {
        Serial.println("Sensor initialization failed");
        while (true) {}
    }

    Serial.println("Sensor initialization successful");
}

void loop() {
    frontSensorArray.updateData(leftSensorDistance,
                            centerSensorDistance,
                            rightSensorDistance);

    Serial.printf("left: %d, center: %d, right: %d\n", 
                  leftSensorDistance, 
                  centerSensorDistance, 
                  rightSensorDistance);

    // delay(500);
}