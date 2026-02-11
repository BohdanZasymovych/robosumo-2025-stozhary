#include <Arduino.h>
#include "config.h"
#include "motor.h"
#include "robot.h"
#include "front_sensor_array.h"
#include "side_sensors.h"
#include "strategy.h"


Motor motorLeft(MOTOR_LEFT_LPWM, MOTOR_LEFT_RPWM);
Motor motorRight(MOTOR_RIGHT_LPWM, MOTOR_RIGHT_RPWM);
Robot robot(motorLeft, motorRight, 255);

FrontSensorArray frontSensorArray;
SideSensors sideSensorArray;
SensorData sensorData;

void setup() {
    Serial.begin(115200);
    delay(5000);
    frontSensorArray.begin();
    sideSensorArray.begin();
}

void loop() {
    frontSensorArray.updateData(sensorData.frontLeftSensorDistance,
                                sensorData.frontCenterSensorDistance,
                                sensorData.frontRightSensorDistance);
    sideSensorArray.updateData(sensorData.sideLeftSensorDistance,
                            sensorData.sideRightSensorDistance);

    Serial.print("Front Left: ");
    Serial.print(sensorData.frontLeftSensorDistance);
    Serial.print("\tFront Center: ");
    Serial.print(sensorData.frontCenterSensorDistance);
    Serial.print("\tFront Right: ");
    Serial.print(sensorData.frontRightSensorDistance);
    Serial.print("\tSide Left: ");
    Serial.print(sensorData.sideLeftSensorDistance);
    Serial.print("\tSide Right: ");
    Serial.print(sensorData.sideRightSensorDistance);
    Serial.print("\n");

    delay(100);

    // executeStrategy(robot, sensorData);
}