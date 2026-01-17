#include <Arduino.h>
#include "motor.h"
#include "robot.h"
#include "pinout.h"

Motor motorLeft(MOTOR_LEFT_LPWM, MOTOR_LEFT_RPWM);
Motor motorRight(MOTOR_RIGHT_LPWM, MOTOR_RIGHT_RPWM);
Robot robot(motorLeft, motorRight, 255);


void setup() {
    delay(5000);
}

void loop() {
    robot.moveForward();
    delay(5000);

    robot.moveBackward();
    delay(5000);

    robot.turnLeft();
    delay(5000);

    robot.turnRight();
    delay(5000);
}