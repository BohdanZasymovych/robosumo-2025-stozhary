#ifndef LADLE_H
#define LADLE_H

#include <ESP32Servo.h>

class Ladle {
public:
    enum State { DOWN, UP, COOLDOWN };
    
    static const int DOWN_ANGLE = 10;
    static const int UP_ANGLE = 70;
    static const uint16_t DIST_NEAR = 150;
    static const uint16_t DIST_FAR = 300;
    static const uint16_t MAX_DIST = 8000;
    
    static const int PRESS_ON_DELTA = 150;
    static const int PRESS_OFF_DELTA = 50;
    static const int CALIB_SAMPLES = 50;
    
    static const int TICKS_PRESS = 3;
    static const int TICKS_FAR = 5;
    
    static const unsigned long COOLDOWN_MS = 500;
    static const unsigned long NEAR_HOLD_MS = 2500;
    static const unsigned long STALE_MS = 300;
    static const unsigned long WARMUP_MS = 500;
    static const unsigned long HOLD_AFTER_LIFT_MS = 800;
    
    Ladle(uint8_t s1Pin, uint8_t s2Pin, uint8_t forcePin);
    void init();
    void update(uint16_t distMm, unsigned long ms);
    State getState() const { return state; }
    int getAngle() const { return angle; }
    bool isUp() const { return state == UP; }

private:
    Servo s1, s2;
    uint8_t s1_pin, s2_pin, force_pin;
    State state;
    int angle;
    
    int force_baseline;
    int force_filtered;
    int calib_count;
    bool is_pressed;
    
    int press_cnt;
    int far_cnt;
    
    unsigned long lower_time;
    unsigned long lift_time;
    unsigned long near_start_time;
    unsigned long last_dist_update_time;
    unsigned long warmup_end_time;
    
    void setAngle(int a);
    void lift();
    void lower();
};

#endif
