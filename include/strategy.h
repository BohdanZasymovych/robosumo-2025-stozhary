#ifndef STRATEGY_H
#define STRATEGY_H


#include <cstdint>
#include "robot.h"

struct SensorData {
    uint16_t frontLeftSensorDistance;
    uint16_t frontCenterSensorDistance;
    uint16_t frontRightSensorDistance;

    uint16_t sideLeftSensorDistance;
    uint16_t sideRightSensorDistance;
};

void executeStrategy(Robot& robot, SensorData& sensorData);


#endif