#include <Arduino.h>
#include "config.h"
#include "motor.h"
#include "robot.h"
#include "front_sensor_array.h"
#include "side_sensors.h"
#include "linesensor.h"
#include "strategy.h"
#include "ladle.h"

#define START_DELAY 5000

Motor motorLeft(MOTOR_LEFT_LPWM, MOTOR_LEFT_RPWM);
Motor motorRight(MOTOR_RIGHT_LPWM, MOTOR_RIGHT_RPWM);
Robot robot(motorLeft, motorRight, 200);

FrontSensorArray frontSensorArray;
SideSensors sideSensorArray;
Linesensor lineLeft(LINE_SENSOR_LEFT);
Linesensor lineRight(LINE_SENSOR_RIGHT);
SensorData sensorData;

Ladle ladle(LADLE_SERVO1_PIN, LADLE_SERVO2_PIN, FORCE_SENSOR1_PIN, FORCE_SENSOR2_PIN);


void setup() {
    unsigned long startTime = millis();

    frontSensorArray.begin();
    sideSensorArray.begin();
    
    while (millis() - startTime < START_DELAY) {}
}


void loop() {
    frontSensorArray.updateData(sensorData.frontLeftSensorDistance,
                                sensorData.frontCenterSensorDistance,
                                sensorData.frontRightSensorDistance);
    sideSensorArray.updateData(sensorData.sideLeftSensorDistance,
                            sensorData.sideRightSensorDistance);

    sensorData.lineLeftDetected = lineLeft.isLineDetected();;
    sensorData.lineRightDetected = lineRight.isLineDetected();;

    // Serial.printf("Front Left: %d ", sensorData.frontLeftSensorDistance);
    // Serial.printf("Front Center: %d ", sensorData.frontCenterSensorDistance);
    // Serial.printf("Front Right: %d ", sensorData.frontRightSensorDistance);
    // Serial.printf("Side Left: %d ", sensorData.sideLeftSensorDistance);
    // Serial.printf("Side Right: %d ", sensorData.sideRightSensorDistance);
    // Serial.printf("Line Left: %d ", sensorData.lineLeftDetected);
    // Serial.printf("Line Right: %d ", sensorData.lineRightDetected);
    // Serial.print("\n");
    
    executeStrategy(robot, sensorData);
}
