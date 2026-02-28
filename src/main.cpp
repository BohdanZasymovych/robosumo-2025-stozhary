#include <Arduino.h>
#include <Wire.h>
#include "config.h"
#include "motor.h"
#include "robot.h"
#include "front_sensor_array.h"
#include "side_sensors.h"
#include "linesensor.h"
#include "strategy.h"
#include "ladle.h"
#include "ladle_distance_sensor.h"
#include "i2c_bus_manager.h"

#define START_DELAY 5000u
#define DEFAULT_SPEED 255u
#define LADLE_UPDATE_PERIOD_MS 50u

I2CBusManager wire0Manager(&Wire, I2C_WIRE_SDA, I2C_WIRE_SCL, I2C_WIRE_CLOCK_FREQ);
I2CBusManager wire1Manager(&Wire1, I2C_WIRE1_SDA, I2C_WIRE1_SCL, I2C_WIRE1_CLOCK_FREQ);

Motor motorLeft(MOTOR_LEFT_LPWM, MOTOR_LEFT_RPWM);
Motor motorRight(MOTOR_RIGHT_LPWM, MOTOR_RIGHT_RPWM);
Robot robot(motorLeft, motorRight, DEFAULT_SPEED);

FrontSensorArray frontSensorArray;
SideSensors sideSensorArray;
LadleDistanceSensor ladleDistanceSensor;

Linesensor lineLeft(LINE_SENSOR_LEFT);
Linesensor lineRight(LINE_SENSOR_RIGHT);
SensorData sensorData;

Ladle ladle(LADLE_SERVO1_PIN, LADLE_SERVO2_PIN, FORCE_SENSOR1_PIN);
unsigned long ladleLastUpdate = 0;

void setup() {
    wire0Manager.begin();
    wire1Manager.begin();

    unsigned long startTime = millis();

    frontSensorArray.begin(&wire0Manager);
    sideSensorArray.begin(&wire1Manager);
    ladleDistanceSensor.begin(&wire1Manager);
    ladle.init();
    
    while (millis() - startTime < START_DELAY) {}
}

void loop() {
    unsigned long now = millis();

    if (wire0Manager.isBusStuck()) {
        wire0Manager.recoverBus();
        frontSensorArray.begin(&wire0Manager);
    } else {
        frontSensorArray.updateData(sensorData.frontLeftSensorDistance,
                                    sensorData.frontCenterSensorDistance,
                                    sensorData.frontRightSensorDistance);
    }

    if (wire1Manager.isBusStuck()) {
        wire1Manager.recoverBus();
        sideSensorArray.begin(&wire1Manager);
        ladleDistanceSensor.begin(&wire1Manager);
    } else {
        sideSensorArray.updateData(sensorData.sideLeftSensorDistance,
                                   sensorData.sideRightSensorDistance);
        
        ladleDistanceSensor.updateData(sensorData.ladleSensorDistance);
    }

    sensorData.lineLeftDetected = lineLeft.isLineDetected();
    sensorData.lineRightDetected = lineRight.isLineDetected();

    if (now - ladleLastUpdate >= LADLE_UPDATE_PERIOD_MS) {
        ladle.update(sensorData.ladleSensorDistance, now);
        ladleLastUpdate = now;
    }

    executeStrategy(robot, sensorData);
}