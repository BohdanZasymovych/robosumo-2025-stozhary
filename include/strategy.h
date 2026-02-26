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

    bool lineLeftDetected;
    bool lineRightDetected;
};

void executeStrategy(Robot& robot, SensorData& sensorData);

#endif
