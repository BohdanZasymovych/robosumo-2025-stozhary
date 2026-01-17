#ifndef MOTOR_H
#define MOTOR_H


#include <cstdint>


class Motor {
    private:
        // Settings of the PWM
        static constexpr int frequency = 2500;
        static constexpr int resolution = 8;
        const uint8_t pwmChannel; // Current PWM channel 
        static uint8_t nextChannel; // Keeps track of the next channel to use

        const uint8_t  forwardPin;
        const uint8_t  backwardPin;
        const uint8_t  speedPin;

    public:
        Motor(uint8_t forwardPin, uint8_t backwardPin, uint8_t speedPin);

        void stop();
        
        void rotateForward(uint8_t speed);
        void rotateBackward(uint8_t speed);
};


#endif