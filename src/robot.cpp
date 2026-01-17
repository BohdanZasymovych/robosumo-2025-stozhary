#include "robot.h"
#include <Arduino.h>


Robot::Robot(Motor& left, Motor& right, uint8_t baseSpeed): leftMotor(left), rightMotor(right), baseSpeed(baseSpeed) {
    leftMotor.stop();
    rightMotor.stop();
}

void Robot::setSpeed(uint8_t newSpeed) {
    baseSpeed = newSpeed;
}

uint8_t Robot::getSpeed() const {
    return baseSpeed;
}

void Robot::moveForward(uint8_t speed) {
    speed = (speed == 0) ? baseSpeed : speed;

    leftMotor.rotateForward(speed);
    rightMotor.rotateForward(speed);
}

void Robot::moveBackward(uint8_t speed) {
    speed = (speed == 0) ? baseSpeed : speed;

    leftMotor.rotateBackward(speed);
    rightMotor.rotateBackward(speed);
}

void Robot::turnLeft(uint8_t speed) {
    speed = (speed == 0) ? baseSpeed : speed;

    leftMotor.rotateBackward(speed);
    rightMotor.rotateForward(speed);
}

void Robot::turnRight(uint8_t speed) {
    speed = (speed == 0) ? baseSpeed : speed;

    leftMotor.rotateForward(speed);
    rightMotor.rotateBackward(speed);
}

void Robot::stop() {
    leftMotor.stop();
    rightMotor.stop();
}