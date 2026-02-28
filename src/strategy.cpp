#include "strategy.h"
#include <Arduino.h>


#define MAX_DISTANCE_MM 1000

static constexpr uint16_t MEMORY_MS = 3000u;

static constexpr uint8_t SPEED_PUSH   = 255; 
static constexpr uint8_t SPEED_ATTACK = 255; 
static constexpr uint8_t SPEED_TURN   = 200;
static constexpr uint8_t SPEED_ROTATE = 175;
static constexpr uint8_t SPEED_ESCAPE_FAST = 255;
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

struct SensorState {
    bool frontCenter;
    bool frontLeft;
    bool frontRight;
    bool sideLeft;
    bool sideRight;
    bool any;
};

static SensorState getSensorState(const SensorData& s) {
    SensorState st;
    st.frontCenter = s.frontCenterSensorDistance < MAX_DISTANCE_MM;
    st.frontLeft   = s.frontLeftSensorDistance < MAX_DISTANCE_MM;
    st.frontRight  = s.frontRightSensorDistance < MAX_DISTANCE_MM;
    st.sideLeft    = s.sideLeftSensorDistance < MAX_DISTANCE_MM;
    st.sideRight   = s.sideRightSensorDistance < MAX_DISTANCE_MM;
    st.any         = st.frontCenter || st.frontLeft || st.frontRight || st.sideLeft || st.sideRight;
    return st;
}

static void updateLastSeen(const SensorState& st, uint32_t now) {
    if (st.sideLeft && !st.sideRight) {
        g_lastDir = LastDir::LEFT;
        g_lastSeenMs = now;
        return;
    }
    
    if (st.sideRight && !st.sideLeft) {
        g_lastDir = LastDir::RIGHT;
        g_lastSeenMs = now;
        return;
    }

    if (st.frontLeft && !st.frontRight) {
        g_lastDir = LastDir::LEFT;
        g_lastSeenMs = now;
        return;
    }
    
    if (st.frontRight && !st.frontLeft) {
        g_lastDir = LastDir::RIGHT;
        g_lastSeenMs = now;
        return;
    }

    if (st.frontCenter || st.any) {
        g_lastSeenMs = now;
    }
}

static void attack(Robot& robot, const SensorData& s, const SensorState& st) {
    if (st.frontCenter) {
        robot.moveForward(SPEED_PUSH);
        return;
    }
  
    if (st.frontLeft && !st.frontRight) {
        if (s.frontLeftSensorDistance <= 70){
            robot.turnLeft(SPEED_ROTATE);
        } else {
            robot.turnMove(SPEED_TURN, SPEED_ATTACK);
        }
       
        return;
    }

    if (st.frontRight && !st.frontLeft) {
        if (s.frontRightSensorDistance <= 70){
            robot.turnRight(SPEED_ROTATE);
        } else { 
            robot.turnMove(SPEED_ATTACK, SPEED_TURN);
        }
        
        return;
    }
   
    if (st.frontLeft && st.frontRight) {
        robot.moveForward(SPEED_ATTACK);
        return;
    }

    // if (st.sideLeft && !st.sideRight) {
    //     robot.turnLeft(SPEED_ROTATE);
    //     return;
    // }

    // if (st.sideRight && !st.sideLeft) {
    //     robot.turnRight(SPEED_ROTATE);
    //     return;
    // }

    if (st.sideLeft && st.sideRight) {
        if (g_lastDir == LastDir::LEFT) robot.turnLeft(SPEED_ROTATE);
        else robot.turnRight(SPEED_ROTATE);
        return;
    }
}

static void search(Robot& robot, uint32_t now) {
    // if ((g_lastSeenMs > 0) && (now - g_lastSeenMs < MEMORY_MS)) {
    //     if (g_lastDir == LastDir::LEFT) {
    //         robot.turnLeft(SPEED_ROTATE);
    //     } else {
    //         robot.turnRight(SPEED_ROTATE);
    //     }
    //     return;
    // }
    
    robot.turnRight(SPEED_ROTATE);
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


void executeStrategy(Robot& robot, SensorData& sensorData) {
    const uint32_t now = millis();

    if (handleLineEscape(robot, sensorData, now)) {
        return;
    }

    SensorState state = getSensorState(sensorData);

    updateLastSeen(state, now);

    if (state.any) {
        attack(robot, sensorData, state);
    } 
   
    else {
        search(robot, now);
    }
}