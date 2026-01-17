#ifndef MOTOR_H
#define MOTOR_H


#include <cstdint>


class Motor {
private:
    // Settings of the PWM
    static constexpr int frequency = 2500;
    static constexpr int resolution = 8;
    const uint8_t PWMChannelForward;
    const uint8_t PWMChannelBackward;
    static uint8_t nextPWMChannel; // Keeps track of the next channel to use

    const uint8_t  forwardPWMPin;
    const uint8_t  backwardPWMPin;

public:
    Motor(uint8_t forwardPWMPin, uint8_t backwardPWMPin);

    void stop();

    void rotateForward(uint8_t speed);
    void rotateBackward(uint8_t speed);
};


#endif