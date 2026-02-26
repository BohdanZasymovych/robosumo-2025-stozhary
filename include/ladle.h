#ifndef LADLE_H
#define LADLE_H

#include <Arduino.h>
#include <ESP32Servo.h>

class Ladle {
private:
    // Сервомотори
    Servo servo1;
    Servo servo2;
    int servo1Pin;
    int servo2Pin;

    // Датчик тиску/сили
    int forceSensorPin;
    
    // Параметри ківша
    const int LADLE_LIFT_ANGLE = 60;        // Кут підйому
    const int LADLE_DOWN_ANGLE = 0;         // Кут опускання
    const int FORCE_THRESHOLD = 200;         // Поріг датчика сили (ADC)
    
    const int CENTER_DROP_DISTANCE = 30;     // 3 см - відстань для опускання
    const int MIDDLE_LIFT_DISTANCE = 5;      // 0.5 см - відстань для підйому
    const int MIDDLE_WAIT_DISTANCE = 20;     // 2 см - відстань для очікування
    
    const int LOWER_STEP = 5;                // Крок опускання в градусах
    const int LOWER_DELAY = 100;             // Затримка між кроками (мс)
    
    // Фільтр для датчика сили
    static const int FORCE_FILTER_SIZE = 5;
    int forceBuffer[FORCE_FILTER_SIZE];
    int forceBufferIndex;
    
    // Стан ківша
    int currentAngle;
    bool isLifted;
    bool waitingForCloserDistance;           // Чекаємо поки центральний датчик покаже 0.5см
    unsigned long lastLowerTime;
    
    // Приватні методи
    int getFilteredForce(int newValue);
    void setAngle(int angle);
    void liftLadle();
    void lowerLadle();
    
public:
    // Конструктор
    Ladle(int s1Pin, int s2Pin, int forceSensorPin);
    
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
