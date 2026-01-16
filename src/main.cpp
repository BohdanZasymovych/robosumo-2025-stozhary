#include <Arduino.h>
#include "motor.h"
#include "robot.h"
#include "front_sensor_array.h"

#define TIMING_BUDGET 20000

uint16_t leftSensorDistance = 0;
uint16_t centerSensorDistance = 0;
uint16_t rightSensorDistance = 0;

FrontSensorArray frontSensorArray;

void setup() {
    Serial.begin(115200);
    
    if (!frontSensorArray.begin(TIMING_BUDGET)) {
        Serial.println("Sensor initialization failed");
    }
}

void loop() {
    frontSensorArray.updateData(leftSensorDistance,
                            centerSensorDistance,
                            rightSensorDistance);

    Serial.printf("left: %d, center: %d, right: %d\n", 
                  leftSensorDistance, 
                  centerSensorDistance, 
                  rightSensorDistance);

    delay(20);
}