#include "robot.h"
#include <Arduino.h>


Robot:: Robot( Motor& left, Motor& right, int baseSpeed): leftMotor(left), rightMotor(right), baseSpeed(baseSpeed){
    if(baseSpeed> MAX_SPEED){
        baseSpeed = MAX_SPEED;
    }else if (baseSpeed < 0) {
    baseSpeed = 0;}
    ;
    leftMotor.stop();
    rightMotor.stop();
}

void Robot :: setSpeed(int newSpeed){
    if (newSpeed <= MAX_SPEED && newSpeed>=0){
        baseSpeed = newSpeed;
    }
}

int Robot :: getSpeed() const{
    return baseSpeed;
}

void Robot :: moveForward(unsigned int speed){
    // if (speed < 0) {
    //     Serial.println("Error: moveForward() received a negative speed value!");
    //     return;

    // }
    if (speed > MAX_SPEED) speed = MAX_SPEED;
    
    speed = (speed == 0) ? baseSpeed : speed;

    leftMotor.move(speed);
    rightMotor.move(speed);

}

void Robot :: moveBackward(unsigned int speed){
   

    int realSpeed = (speed == 0) ? -baseSpeed : -static_cast<int>(speed);
    if (abs(realSpeed) > MAX_SPEED) realSpeed = -MAX_SPEED;

    leftMotor.move(realSpeed);
    rightMotor.move(realSpeed);
}

void Robot :: turnLeft(unsigned int speed){

    speed = (speed == 0) ? baseSpeed : speed;
    if (speed > MAX_SPEED) speed = MAX_SPEED;
    int realSpeed = static_cast<int>(speed);
    leftMotor.move(-realSpeed);
    rightMotor.move(realSpeed);

}
void Robot :: turnRight(unsigned int speed){

    speed = (speed == 0) ? baseSpeed : speed;
    if (speed > MAX_SPEED) speed = MAX_SPEED;
    int realSpeed = static_cast<int>(speed);
    leftMotor.move(realSpeed);
    rightMotor.move(-realSpeed);

}
void Robot::stop(){
    leftMotor.stop();
    rightMotor.stop();
    
}


