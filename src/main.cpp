#include <Arduino.h>
#include "pinout.h"
#include "motor.h"
#include "robot.h"
#include "front_sensor_array.h"
#include "strategy.h"
#include "ladle.h"

Motor motorLeft(MOTOR_LEFT_LPWM, MOTOR_LEFT_RPWM);
Motor motorRight(MOTOR_RIGHT_LPWM, MOTOR_RIGHT_RPWM);
Robot robot(motorLeft, motorRight, 255);

FrontSensorArray frontSensorArray;
SensorData sensorData;

Ladle ladle(LADLE_SERVO1_PIN, LADLE_SERVO2_PIN, FORCE_SENSOR_PIN);

void setup() {
    delay(5000);
    frontSensorArray.begin();
    ladle.init();
}

void loop() {
    frontSensorArray.updateData(sensorData.frontLeftSensorDistance,
                                sensorData.frontCenterSensorDistance,
                                sensorData.frontRightSensorDistance);
    
    // Оновлення ківша (використовується лише центральний датчик)
    ladle.update(sensorData.frontCenterSensorDistance);
    
    executeStrategy(robot, sensorData);
}