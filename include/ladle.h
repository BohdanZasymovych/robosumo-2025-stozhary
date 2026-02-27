#ifndef LADLE_H
#define LADLE_H

#include <Arduino.h>
#include <ESP32Servo.h>

class Ladle {
private:
    // Сервомотори (автоматично використовують PWM канали 0-1)
    Servo servo1;
    Servo servo2;
    const uint8_t servo1Pin;
    const uint8_t servo2Pin;

    // Датчики тиску/сили
    int forceSensor1Pin;  // Верхній: старт 0-100, натиск >= 170
    int forceSensor2Pin;  // Нижній: 0=немає, натиск >= 60
    
    // Параметри ківша
    const int LADLE_LIFT_ANGLE = 60;
    const int LADLE_DOWN_ANGLE = 20;  // Дно ківша на 20° (замість 0°)
    const int CENTER_DROP_DISTANCE = 30;  // 3 см
    
    // Пороги тиску
    const int SENSOR1_PRESS_THRESHOLD = 170; // Верхній: натиск >= 170
    const int SENSOR2_PRESS_THRESHOLD = 60;  // Нижній: натиск >= 60
    
    // Стан ківша
    int currentAngle;
    bool isLifted;
    bool waitingForCloserDistance;
    unsigned long lastLowerTime;
    
    // Приватні методи
    void setAngle(int angle);
    void liftLadle();
    void lowerLadle();
    bool detectSpikeAndPressure(int sensor1Val, int sensor2Val);
    
public:
    // Конструктор
    Ladle(int s1Pin, int s2Pin, int fs1Pin, int fs2Pin);
    
    // Ініціалізація
    void init();
    
    // Основна функція оновлення (викликається в main loop)
    // centerDistance - відстань центрального датчика (мм)
    void update(uint16_t centerDistance);
    
    // Отримання поточного стану
    bool isUp() const { return isLifted; }
    int getCurrentAngle() const { return currentAngle; }
};

#endif // LADLE_H
