#include "strategy.h"

#define MAX_DISTANCE_MM 1200

void executeStrategy(Robot& robot, SensorData& sensorData) {
    if (sensorData.frontCenterSensorDistance < MAX_DISTANCE_MM) {
        robot.moveForward();
    } else if (sensorData.frontLeftSensorDistance < MAX_DISTANCE_MM) {
        // move front and turn right
    } else if (sensorData.frontRightSensorDistance < MAX_DISTANCE_MM) {
        // move front and turn left
    }
}
