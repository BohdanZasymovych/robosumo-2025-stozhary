#include "ladle.h"
#include <Arduino.h>

Ladle::Ladle(uint8_t s1Pin, uint8_t s2Pin, uint8_t forcePin)
    : s1_pin(s1Pin), s2_pin(s2Pin), force_pin(forcePin),
      state(DOWN), angle(DOWN_ANGLE),
      force_baseline(0), force_filtered(0), calib_count(0), is_pressed(false),
      press_cnt(0), far_cnt(0),
      lower_time(0), lift_time(0), near_start_time(0), last_dist_update_time(0), warmup_end_time(0) {}

void Ladle::init() {
    s1.attach(s1_pin);
    s2.attach(s2_pin);
    pinMode(force_pin, INPUT);
    setAngle(DOWN_ANGLE);
    warmup_end_time = millis() + WARMUP_MS;
}

void Ladle::setAngle(int a) {
    angle = a;
    s1.write(a);
    s2.write(180 - a);
}

void Ladle::lift() {
    if (state != UP) {
        state = UP;
        setAngle(UP_ANGLE);
        lift_time = millis();
    }
}

void Ladle::lower() {
    if (state != COOLDOWN) {
        state = COOLDOWN;
        setAngle(DOWN_ANGLE);
        lower_time = millis();
    }
}

void Ladle::update(uint16_t distMm, unsigned long ms) {
    int raw = analogRead(force_pin);
    
    if (calib_count < CALIB_SAMPLES) {
        if (calib_count == 0) {
            force_baseline = raw;
            force_filtered = raw;
        } else {
            force_baseline += raw;
        }
        calib_count++;
        if (calib_count == CALIB_SAMPLES) {
            force_baseline /= CALIB_SAMPLES;
            force_filtered = force_baseline;
        }
        return;
    }
    
    force_filtered = (raw + 4 * force_filtered) / 5;
    int delta = force_filtered - force_baseline;
    
    if (delta >= PRESS_ON_DELTA) is_pressed = true;
    else if (delta <= PRESS_OFF_DELTA) is_pressed = false;
    
    if (ms < warmup_end_time) return;
    
    bool dist_valid = (distMm > 0 && distMm < MAX_DIST);
    if (dist_valid) last_dist_update_time = ms;
    bool dist_stale = (ms - last_dist_update_time) > STALE_MS;
    
    bool is_near = dist_valid && !dist_stale && (distMm <= DIST_NEAR);
    bool is_far = !dist_valid || dist_stale || (distMm >= DIST_FAR);
    
    switch (state) {
        case DOWN:
            if (is_pressed && is_near) {
                press_cnt++;
                near_start_time = 0;
                if (press_cnt >= TICKS_PRESS) {
                    lift();
                    press_cnt = 0;
                    far_cnt = 0;
                }
            } else {
                press_cnt = 0;
                
                if (is_near && !is_pressed) {
                    if (near_start_time == 0) {
                        near_start_time = ms;
                    } else if (ms - near_start_time >= NEAR_HOLD_MS) {
                        lift();
                        near_start_time = 0;
                        far_cnt = 0;
                    }
                } else {
                    near_start_time = 0;
                }
            }
            break;
            
        case UP:
            if (ms - lift_time < HOLD_AFTER_LIFT_MS) {
                far_cnt = 0;
            } else if (!is_pressed) {
                far_cnt++;
                if (far_cnt >= TICKS_FAR || is_far) {
                    lower();
                    far_cnt = 0;
                    press_cnt = 0;
                }
            } else {
                far_cnt = 0;
            }
            break;
            
        case COOLDOWN:
            if (ms - lower_time >= COOLDOWN_MS) {
                state = DOWN;
                press_cnt = 0;
                far_cnt = 0;
                near_start_time = 0;
            }
            break;
    }
}
