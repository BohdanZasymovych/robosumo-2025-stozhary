#include <Arduino.h>
#include "pinout.h"


#include "motor.h"
#include "robot.h"
#include "front_sensor_array.h"
#include "strategy.h"




Motor motorLeft(MOTOR_LEFT_LPWM, MOTOR_LEFT_RPWM);
Motor motorRight(MOTOR_RIGHT_LPWM, MOTOR_RIGHT_RPWM);
Robot robot(motorLeft, motorRight, 255);

FrontSensorArray frontSensorArray;
SensorData sensorData;

void setup() {
    delay(5000);
    frontSensorArray.begin();
}

void loop() {
    frontSensorArray.updateData(sensorData.frontLeftSensorDistance,
                                sensorData.frontCenterSensorDistance,
                                sensorData.frontRightSensorDistance);
    
    executeStrategy(robot, sensorData);
}