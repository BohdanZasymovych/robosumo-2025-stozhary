#ifndef MOTOR_H
#define MOTOR_H

#include <cinttypes>

class Motor {
    private:
        // Settings of PWM
        static constexpr int frequency = 2500;
        static constexpr int resolution = 8;
        const uint8_t pwmChannel; // Current PWN channel 
        static uint8_t nextChannel; // Keeps track of the nexr channel to use

        // Pin mapping
        const uint8_t  forwardPin;
        const uint8_t  backwardPin;
        const uint8_t  speedPin;

    public:
        Motor(uint8_t forwardPin, uint8_t backwardPin, uint8_t speedPin);

        // Stops the motor if it is rotating
        void stop();
        
        // speed - integer in range from -255 to 255
        // If speed is positive motor rotates forward, if negative backward
        // If speed is 0 motor stops
        void move(int speed);

};

#endif
