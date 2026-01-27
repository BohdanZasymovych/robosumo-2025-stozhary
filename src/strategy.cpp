#include "strategy.h"
#include <Arduino.h>


#define MAX_DISTANCE_MM 1200
#define  MEMORY_MS 1200

static constexpr uint8_t SPEED_PUSH   = 220; 
static constexpr uint8_t SPEED_ATTACK = 200; 
static constexpr uint8_t SPEED_TURN   = 140;
static constexpr uint8_t SPEED_ROTATE = 160; 


enum class LastDir : uint8_t { LEFT, RIGHT };


static LastDir g_lastDir = LastDir::RIGHT;
static uint32_t g_lastSeenMs = 0;

static inline bool seesFrontCenter(const SensorData& s) {
    return s.frontCenterSensorDistance < MAX_DISTANCE_MM;
}
static inline bool seesFrontLeft(const SensorData& s) {
    return s.frontLeftSensorDistance < MAX_DISTANCE_MM;
}
static inline bool seesFrontRight(const SensorData& s) {
    return s.frontRightSensorDistance < MAX_DISTANCE_MM;
}
static inline bool seesSideLeft(const SensorData& s) {
    return s.sideLeftSensorDistance < MAX_DISTANCE_MM;
}
static inline bool seesSideRight(const SensorData& s) {
    return s.sideRightSensorDistance < MAX_DISTANCE_MM;
}

static inline bool seesAny(const SensorData& s) {
    return seesFrontCenter(s) || seesFrontLeft(s) || seesFrontRight(s) || seesSideLeft(s) || seesSideRight(s);
}

static void updateLastSeen(const SensorData& s, uint32_t now) {
    
    if (seesSideLeft(s) && !seesSideRight(s)) {
        g_lastDir = LastDir::LEFT;
        g_lastSeenMs = now;
        return;
    }
    if (seesSideRight(s) && !seesSideLeft(s)) {
        g_lastDir = LastDir::RIGHT;
        g_lastSeenMs = now;
        return;
    }

 
    if (seesFrontLeft(s) && !seesFrontRight(s)) {
        g_lastDir = LastDir::LEFT;
        g_lastSeenMs = now;
        return;
    }
    if (seesFrontRight(s) && !seesFrontLeft(s)) {
        g_lastDir = LastDir::RIGHT;
        g_lastSeenMs = now;
        return;
    }

    if (seesFrontCenter(s) || seesAny(s)) {
        g_lastSeenMs = now;
    }
}



static void attack(Robot& robot, const SensorData& s) {
    const bool front_C = seesFrontCenter(s);
    const bool front_L = seesFrontLeft(s);
    const bool front_R = seesFrontRight(s);
    const bool side_L = seesSideLeft(s);
    const bool side_R = seesSideRight(s);

    if (front_C) {
        robot.moveForward(SPEED_PUSH);
        return;
    }

  
    if (front_L && !front_R) {
        robot.turnMove(SPEED_TURN, SPEED_ATTACK);
        return;
    }
    if (front_R && !front_L) {
        robot.turnMove(SPEED_ATTACK, SPEED_TURN);
        return;
    }

   
    if (front_L && front_R) {
        robot.moveForward(SPEED_ATTACK);
        return;
    }

    if (side_L && !side_R) {
        robot.turnLeft(SPEED_ROTATE);
        return;
    }
    if (side_R && !side_L) {
        robot.turnRight(SPEED_ROTATE);
        return;
    }

   
    if (side_L && side_R) {
        if (g_lastDir == LastDir::LEFT) robot.turnLeft(SPEED_ROTATE);
        else robot.turnRight(SPEED_ROTATE);
        return;
    }

 
    robot.stop();
}

static void search(Robot& robot, uint32_t now) {
   
    if (now - g_lastSeenMs < MEMORY_MS) {
        if (g_lastDir == LastDir::LEFT) {
            robot.turnLeft(SPEED_ROTATE);
        } else {
            robot.turnRight(SPEED_ROTATE);
        }
        return;
    }

 
    const uint32_t t = now % 1500;

    if (t < 600) {
        robot.turnRight(SPEED_ROTATE);
    } else if (t < 1200) {
        robot.turnLeft(SPEED_ROTATE);
    } else {
        robot.moveForward(150); 
    }
}


// void executeStrategy(Robot& robot, SensorData& sensorData) {
//     if (sensorData.frontCenterSensorDistance < MAX_DISTANCE_MM) {
//         robot.moveForward();
//     } else if (sensorData.frontLeftSensorDistance < MAX_DISTANCE_MM) {
//         // move front and turn right
//     } else if (sensorData.frontRightSensorDistance < MAX_DISTANCE_MM) {
//         // move front and turn left
//     }
// }

void executeStrategy(Robot& robot, SensorData& sensorData) {
    const uint32_t now = millis();

    updateLastSeen(sensorData, now);

    if (seesAny(sensorData)) {
        attack(robot, sensorData);
    } else {
        search(robot, now);
    }
}


