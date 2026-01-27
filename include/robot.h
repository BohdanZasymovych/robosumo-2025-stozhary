#ifndef ROBOT_H
#define ROBOT_H


#include "motor.h"


class Robot  {
    static constexpr uint8_t MAX_SPEED = 255;

private:
    Motor& leftMotor;
    Motor& rightMotor;
    uint8_t baseSpeed;

public:
    Robot(Motor& left, Motor& right, uint8_t baseSpeed = 200);
    
    void setSpeed(uint8_t newSpeed);
    uint8_t getSpeed() const;

    void moveForward(uint8_t baseSpeed = 0);
    void moveBackward(uint8_t baseSpeed = 0);
    void turnLeft(uint8_t baseSpeed = 0);
    void turnRight(uint8_t baseSpeed = 0);
    void turnMove(uint8_t leftSpeed, uint8_t rightSpeed);
    void stop();

};


#endif