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

// FrontSensorArray frontSensorArray;
// SideSensors sideSensorArray;
Linesensor lineLeft(LINE_SENSOR_LEFT);
Linesensor lineRight(LINE_SENSOR_RIGHT);
SensorData sensorData;

Ladle ladle(LADLE_SERVO1_PIN, LADLE_SERVO2_PIN, FORCE_SENSOR1_PIN);

unsigned long ladleLastUpdate = 0;
unsigned long forceDebugLastPrint = 0;


void setup() {
    unsigned long startTime = millis();

    Serial.begin(115200);

    // frontSensorArray.begin();
    // sideSensorArray.begin();
    ladle.init();
    
    while (millis() - startTime < START_DELAY) {}
}


void loop() {
    // frontSensorArray.updateData(sensorData.frontLeftSensorDistance,
    //                             sensorData.frontCenterSensorDistance,
    //                             sensorData.frontRightSensorDistance);
    // sideSensorArray.updateData(sensorData.sideLeftSensorDistance,
    //                         sensorData.sideRightSensorDistance,
    //                         sensorData.ladleInnerDistanceMm);

    sensorData.lineLeftDetected = lineLeft.isLineDetected();
    sensorData.lineRightDetected = lineRight.isLineDetected();

    unsigned long now = millis();
    if (now - ladleLastUpdate >= 50) {
        ladle.update(340, now); // sensorData.ladleInnerDistanceMm
        ladleLastUpdate = now;
    }

    if (now - forceDebugLastPrint >= 100) {
        int forceRaw = analogRead(FORCE_SENSOR1_PIN);
        Serial.print("FORCE_RAW=");
        Serial.println(forceRaw);
        forceDebugLastPrint = now;
    }
    
    // executeStrategy(robot, sensorData);
}