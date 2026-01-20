#include <Arduino.h>
#include "motor.h"
#include "robot.h"
#include "pinout.h"

Motor motorLeft(MOTOR_LEFT_LPWM, MOTOR_LEFT_RPWM);
Motor motorRight(MOTOR_RIGHT_LPWM, MOTOR_RIGHT_RPWM);
Robot robot(motorLeft, motorRight, 100);


void setup() {}

void loop() {}