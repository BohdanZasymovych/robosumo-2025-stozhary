#include "robot.h"
#include <Arduino.h>

#define LEFT_MOTOR_START_DELAY_MS 80
#define LEFT_MOTOR_PRELOAD_SPEED 20
#define RIGHT_TILT_FIX_CONSTANT 0.062f

Robot::Robot(Motor& left, Motor& right, uint8_t baseSpeed)
    : leftMotor(left)
    , rightMotor(right)
    , baseSpeed(baseSpeed) {
    stop(); 
}

void Robot::setSpeed(uint8_t newSpeed) {
    baseSpeed = newSpeed;
}

uint8_t Robot::getSpeed() const {
    return baseSpeed;
}

void Robot::moveForward(uint8_t speed) {
    speed = (speed == 0) ? baseSpeed : speed;

    if (currentForwardSpeed == speed) {
        return; 
    }

    bool startingFromStop = (currentForwardSpeed == 0);
    currentForwardSpeed = speed; 

    leftCruiseSpeed = static_cast<uint8_t>((1 - RIGHT_TILT_FIX_CONSTANT) * speed);
    uint8_t rightCruiseSpeed = static_cast<uint8_t>(std::min(255u, static_cast<uint>((1 + RIGHT_TILT_FIX_CONSTANT) * speed)));

    if (startingFromStop) {
        rightMotor.rotateForward(rightCruiseSpeed); 
        
        leftMotor.rotateForward(LEFT_MOTOR_PRELOAD_SPEED);
        
        kickstartTimer.once_ms(LEFT_MOTOR_START_DELAY_MS, Robot::timerCallback, this);
    } else {
        rightMotor.rotateForward(rightCruiseSpeed);
        leftMotor.rotateForward(leftCruiseSpeed);
    }
}

void Robot::moveBackward(uint8_t speed) {
    currentForwardSpeed = 0;
    kickstartTimer.detach();

    speed = (speed == 0) ? baseSpeed : speed;

    leftMotor.rotateBackward(speed);
    rightMotor.rotateBackward(speed);
}

void Robot::turnLeft(uint8_t speed) {
    currentForwardSpeed = 0;
    kickstartTimer.detach();

    speed = (speed == 0) ? baseSpeed : speed;

    leftMotor.rotateBackward(speed);
    rightMotor.rotateForward(speed);
}

void Robot::turnRight(uint8_t speed) {
    currentForwardSpeed = 0;
    kickstartTimer.detach();

    speed = (speed == 0) ? baseSpeed : speed;

    leftMotor.rotateForward(speed);
    rightMotor.rotateBackward(speed);
}

void Robot::turnMove(uint8_t leftSpeed, uint8_t rightSpeed) {
    currentForwardSpeed = 0;
    kickstartTimer.detach();

    leftMotor.rotateForward(leftSpeed);
    rightMotor.rotateForward(rightSpeed);
}

void Robot::stop() {
    currentForwardSpeed = 0;
    kickstartTimer.detach();

    leftMotor.stop();
    rightMotor.stop();
}

void Robot::endKickstart() {
    leftMotor.rotateForward(leftCruiseSpeed);
}

void Robot::timerCallback(Robot* instance) {
    instance->endKickstart();
}