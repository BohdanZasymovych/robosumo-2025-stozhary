#include <Arduino.h>
#include "config.h"
#include "motor.h"
#include "robot.h"
#include "front_sensor_array.h"
#include "side_sensors.h"
#include "linesensor.h"
#include "strategy.h"
#include "wifi_debug.h"


Motor motorLeft(MOTOR_LEFT_LPWM, MOTOR_LEFT_RPWM);
Motor motorRight(MOTOR_RIGHT_LPWM, MOTOR_RIGHT_RPWM);
Robot robot(motorLeft, motorRight, 100);

FrontSensorArray frontSensorArray;
SideSensors sideSensorArray;
Linesensor lineLeft(LINE_SENSOR_LEFT);
Linesensor lineRight(LINE_SENSOR_RIGHT);
SensorData sensorData;
unsigned long startTime;
static constexpr uint8_t SPEED_PUSH   = 220; 

void setup() {
    Serial.begin(115200);
    
    WiFiDebug::beginAP();              // використає "ROBO_DEBUG"


    startTime = millis();
    frontSensorArray.begin();
    sideSensorArray.begin();
    
    while (millis() - startTime < 5000)
    {
  
    }
   

    }



void loop() {
    const bool lineLeftDetected = lineLeft.isLineDetected();
    const bool lineRightDetected = lineRight.isLineDetected();

    // static uint32_t lastLineLog = 0;
    // if (millis() - lastLineLog > 100) {
    //     lastLineLog = millis();
    //     Serial.print("Line L: ");
    //     Serial.print(lineLeftDetected);
    //     Serial.print("\tLine R: ");
    //     Serial.println(lineRightDetected);
    // }

    WiFiDebug::update();
    frontSensorArray.updateData(sensorData.frontLeftSensorDistance,
                                sensorData.frontCenterSensorDistance,
                                sensorData.frontRightSensorDistance);
    sideSensorArray.updateData(sensorData.sideLeftSensorDistance,
                            sensorData.sideRightSensorDistance);
    sensorData.lineLeftDetected = lineLeftDetected;
    sensorData.lineRightDetected = lineRightDetected;

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
  
    // static uint32_t lastLog = 0;
    // if (millis() - lastLog > 100) {
    //     lastLog = millis();
    //     WIFI_PRINTF("FL:%u FC:%u FR:%u SL:%u SR:%u\n",
    //         sensorData.frontLeftSensorDistance,
    //         sensorData.frontCenterSensorDistance,
    //         sensorData.frontRightSensorDistance,
    //         sensorData.sideLeftSensorDistance,
    //         sensorData.sideRightSensorDistance
    //     );
    // }
    executeStrategy(robot, sensorData);
    // // robot.moveForward(SPEED_PUSH);
    // robot.moveForward(120);
}
