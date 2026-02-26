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

    // Датчики тиску/сили
    int forceSensor1Pin;  // Простий датчик: 0/20
    int forceSensor2Pin;  // Аналоговий з калібруванням
    
    // Параметри ківша
    const int LADLE_LIFT_ANGLE = 60;
    const int LADLE_DOWN_ANGLE = 0;
    const int CENTER_DROP_DISTANCE = 30;  // 3 см
    
    // Параметри датчика 1 (бінарний: 0=немає, >0=натиск)
    const int SENSOR1_PRESS_THRESHOLD = 2;  // > 2 = натиск (макс 20-25)
    
    // Параметри датчика 2 (аналоговий)
    const int CALIBRATION_ITERATIONS = 20;   // Перші 20 ітерацій для калібрування
    const int SPIKE_THRESHOLD = 100;         // Різкий скачок +100
    const int PRESSURE_HIGH_LEVEL = 450;     // > 450 = тіло на ковші
    const int STABILIZATION_DELTA = 5;       // Падіння < 5 = стабілізація
    
    // Калібрування і стан
    int calibrationCounter;
    int sensor2BaseLevel;          // Базовий рівень 0 для датчика 2
    int sensor2PrevValue;          // Попереднє значення
    bool pressureDetected;         // Чи засічено натиск
    int stableCount;               // Лічильник стабільних значень
    
    // Стан ківша
    int currentAngle;
    bool isLifted;
    bool waitingForCloserDistance;           // Чекаємо поки центральний датчик покаже 0.5см
    unsigned long lastLowerTime;
    
    // Приватні методи
    void setAngle(int angle);
    void liftLadle();
    void lowerLadle();
    bool detectSpikeAndPressure(int sensor1Val, int sensor2Val);
    bool isPressureStillPresent(int sensor2Val);
    
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
