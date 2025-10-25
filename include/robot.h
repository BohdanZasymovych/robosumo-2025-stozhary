#ifndef ROBOT_H
#define ROBOT_H

#include "motor.h"

class Robot  {
    static constexpr int MAX_SPEED = 255;
    private:
        Motor leftMotor;
        Motor rightMotor;
        int baseSpeed;

    public :

    Robot(Motor& left, Motor& right, int baseSpeed = 200);
    
    void setSpeed(int newSpeed);

    int getSpeed() const;

    void moveForward(unsigned int baseSpeed = 0);
    void moveBackward(unsigned int  = 0);
    void turnLeft(unsigned int baseSpeed = 0);
    void turnRight(unsigned int baSespeed = 0);
    void stop();

};
#endif
