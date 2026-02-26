#include "strategy.h"
#include <Arduino.h>


#define MAX_DISTANCE_MM 900
#define  MEMORY_MS 1200

static constexpr uint8_t SPEED_PUSH   = 220; 
static constexpr uint8_t SPEED_ATTACK = 200; 
static constexpr uint8_t SPEED_TURN   = 140;
static constexpr uint8_t SPEED_ROTATE = 160; 
static constexpr uint8_t SPEED_ESCAPE_FAST = 230;
static constexpr uint8_t SPEED_ESCAPE_SLOW = 150;
static constexpr uint16_t LINE_ESCAPE_MS = 240;
static constexpr uint8_t LINE_DEBOUNCE_COUNT = 2;


enum class LastDir : uint8_t { LEFT, RIGHT };


static LastDir g_lastDir = LastDir::RIGHT;
static uint32_t g_lastSeenMs = 0;

enum class LineEscapeMode : uint8_t { STRAIGHT, CURVE_LEFT, CURVE_RIGHT };
static LineEscapeMode g_lineEscapeMode = LineEscapeMode::STRAIGHT;
static uint32_t g_lineEscapeUntilMs = 0;
static uint8_t g_lineLeftStreak = 0;
static uint8_t g_lineRightStreak = 0;
static bool g_lineLeftArmed = true;
static bool g_lineRightArmed = true;

static inline bool timeNotExpired(uint32_t now, uint32_t deadline) {
    return static_cast<int32_t>(deadline - now) > 0;
}

static inline uint8_t nextStreak(uint8_t current, bool detected) {
    if (!detected) return 0;
    return (current < LINE_DEBOUNCE_COUNT) ? static_cast<uint8_t>(current + 1) : current;
}

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
        if (s.frontLeftSensorDistance <= 70){
            robot.turnLeft(SPEED_ROTATE);
            
        }
        else {
            robot.turnMove(SPEED_TURN, SPEED_ATTACK);
        }
       
        return;
    }
    if (front_R && !front_L) {
        if (s.frontRightSensorDistance <= 70){
            robot.turnRight(SPEED_ROTATE);
        }
        else { 
            robot.turnMove(SPEED_ATTACK, SPEED_TURN);
        }
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

   
    // if (side_L && side_R) {
    //     if (g_lastDir == LastDir::LEFT) robot.turnLeft(SPEED_ROTATE);
    //     else robot.turnRight(SPEED_ROTATE);
    //     return;
    // }

 
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

static void applyLineEscape(Robot& robot) {
    switch (g_lineEscapeMode) {
        case LineEscapeMode::STRAIGHT:
            robot.moveForward(SPEED_ESCAPE_FAST);
            break;
        case LineEscapeMode::CURVE_LEFT:
            robot.turnMove(SPEED_ESCAPE_SLOW, SPEED_ESCAPE_FAST);
            break;
        case LineEscapeMode::CURVE_RIGHT:
            robot.turnMove(SPEED_ESCAPE_FAST, SPEED_ESCAPE_SLOW);
            break;
    }
}

static bool handleLineEscape(Robot& robot, const SensorData& s, uint32_t now) {
    g_lineLeftStreak = nextStreak(g_lineLeftStreak, s.lineLeftDetected);
    g_lineRightStreak = nextStreak(g_lineRightStreak, s.lineRightDetected);

    const bool lineLeftDebounced = g_lineLeftStreak >= LINE_DEBOUNCE_COUNT;
    const bool lineRightDebounced = g_lineRightStreak >= LINE_DEBOUNCE_COUNT;
    const bool newLeftTrigger = lineLeftDebounced && g_lineLeftArmed;
    const bool newRightTrigger = lineRightDebounced && g_lineRightArmed;

    if (newLeftTrigger || newRightTrigger) {
        if (lineLeftDebounced && lineRightDebounced) {
            g_lineEscapeMode = LineEscapeMode::STRAIGHT;
        } else if (lineLeftDebounced) {
            g_lineEscapeMode = LineEscapeMode::CURVE_RIGHT;
        } else {
            g_lineEscapeMode = LineEscapeMode::CURVE_LEFT;
        }
        g_lineEscapeUntilMs = now + LINE_ESCAPE_MS;
    }

    
    g_lineLeftArmed = !lineLeftDebounced;
    g_lineRightArmed = !lineRightDebounced;

    if (timeNotExpired(now, g_lineEscapeUntilMs)) {
        applyLineEscape(robot);
        return true;
    }

    return false;
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

    // if (handleLineEscape(robot, sensorData, now)) {
    //     return;
    // }

    updateLastSeen(sensorData, now);

    if (seesAny(sensorData)) {
        attack(robot, sensorData);
    
    } 
   
    else {
        search(robot, now);  // if (front_R && !front_L) {
    //     if (s.frontRightSensorDistance <= 70){
    //         robot.turnRight(SPEED_ROTATE);
    //     }
    //     else { 
    //         robot.turnMove(SPEED_ATTACK, SPEED_TURN);
    //     }
    //     return;
       
    // }
    }
}
