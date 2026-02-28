#include "motor.h"
#include <Arduino.h>


uint8_t Motor::nextPWMChannel = 2; // Start from the second channel, leaving channels 0, 1 for the servos

Motor::Motor(uint8_t forwardPWMPin, uint8_t backwardPWMPin) :
    forwardPWMPin(forwardPWMPin),
    backwardPWMPin(backwardPWMPin),
    PWMChannelForward(Motor::nextPWMChannel++),
    PWMChannelBackward(Motor::nextPWMChannel++) {

        ledcSetup(PWMChannelForward, frequency, resolution);
        ledcAttachPin(forwardPWMPin, PWMChannelForward);

        ledcSetup(PWMChannelBackward, frequency, resolution);
        ledcAttachPin(backwardPWMPin, PWMChannelBackward);

        ledcWrite(PWMChannelForward, 0);
        ledcWrite(PWMChannelBackward, 0);
}

void Motor::stop() {
    ledcWrite(PWMChannelForward, 0);
    ledcWrite(PWMChannelBackward, 0);
}

void Motor::rotateForward(uint8_t speed) {
    ledcWrite(PWMChannelBackward, 0);
    ledcWrite(PWMChannelForward, speed);
}

void Motor::rotateBackward(uint8_t speed) {
    ledcWrite(PWMChannelForward, 0);
    ledcWrite(PWMChannelBackward, speed);
}