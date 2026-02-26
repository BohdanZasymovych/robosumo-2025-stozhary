#include "ladle.h"

Ladle::Ladle(int s1Pin, int s2Pin, int fs1Pin, int fs2Pin)
    : servo1Pin(s1Pin),
      servo2Pin(s2Pin),
      forceSensor1Pin(fs1Pin),
      forceSensor2Pin(fs2Pin),
      currentAngle(LADLE_DOWN_ANGLE),
      isLifted(false),
      waitingForCloserDistance(false),
      lastLowerTime(0),
      calibrationCounter(0),
      sensor2BaseLevel(0),
      sensor2PrevValue(0),
      pressureDetected(false),
      stableCount(0) {
}

void Ladle::init() {
    servo1.attach(servo1Pin);
    servo2.attach(servo2Pin);
    
    pinMode(forceSensor1Pin, INPUT);
    pinMode(forceSensor2Pin, INPUT);
    
    setAngle(LADLE_DOWN_ANGLE);
    
    Serial.println("\n=== LADLE INIT ===");
    Serial.println("2 датчики тиску: простий (0/20) + аналоговий (калібрування)");
    Serial.println("Калібрування перших 20 ітерацій...");
}

void Ladle::setAngle(int angle) {
    currentAngle = angle;
    servo1.write(currentAngle);
    servo2.write(180 - currentAngle);
}

void Ladle::liftLadle() {
    if (!isLifted) {
        Serial.println("[LADLE] ПІДЙОМ на 60°");
        setAngle(LADLE_LIFT_ANGLE);
        isLifted = true;
        pressureDetected = true;
    }
}

void Ladle::lowerLadle() {
    if (isLifted) {
        Serial.println("[LADLE] ОПУСКАННЯ");
        setAngle(LADLE_DOWN_ANGLE);
        isLifted = false;
        pressureDetected = false;
        stableCount = 0;
    }
}

// Детекція різкого скачка та натиску
bool Ladle::detectSpikeAndPressure(int sensor1Val, int sensor2Val) {
    // Датчик 1: бінарний - 0=немає тиску, будь-яке значення >0 (до 20-25) = тиск противника
    if (sensor1Val > SENSOR1_PRESS_THRESHOLD) {
        Serial.print("[SENSOR1] ТИСК ПРОТИВНИКА! Val=");
        Serial.println(sensor1Val);
        return true;
    }
    
    // Датчик 2: аналоговий - шукаємо різкий скачок
    if (calibrationCounter >= CALIBRATION_ITERATIONS) {
        int delta = sensor2Val - sensor2PrevValue;
        
        // Різкий скачок вгору на 100+ одиниць
        if (delta >= SPIKE_THRESHOLD) {
            Serial.print("[SENSOR2] РІЗКИЙ СКАЧОК! Було=");
            Serial.print(sensor2PrevValue);
            Serial.print(", Стало=");
            Serial.print(sensor2Val);
            Serial.print(", Delta=");
            Serial.println(delta);
            return true;
        }
    }
    
    return false;
}

// Перевірка чи тиск все ще присутній (для датчика 2)
bool Ladle::isPressureStillPresent(int sensor2Val) {
    if (calibrationCounter < CALIBRATION_ITERATIONS) {
        return false; // Ще калібруємось
    }
    
    // Якщо > 450 і тримається стабільно - тіло ще на ковші
    if (sensor2Val > PRESSURE_HIGH_LEVEL) {
        Serial.print("[SENSOR2] Високий рівень! Val=");
        Serial.println(sensor2Val);
        return true;
    }
    
    // Якщо падає потихеньку (< 5 одиниць) - стабілізація, тиску немає
    int delta = abs(sensor2Val - sensor2PrevValue);
    if (delta <= STABILIZATION_DELTA) {
        stableCount++;
        if (stableCount > 3) {
            Serial.println("[SENSOR2] Стабілізація - тиску немає");
            return false;
        }
    } else {
        stableCount = 0;
    }
    
    return pressureDetected; // Тримаємо попередній стан
}

void Ladle::update(uint16_t centerDistance) {
    // Зчитуємо обидва датчики
    int sensor1 = analogRead(forceSensor1Pin);
    int sensor2 = analogRead(forceSensor2Pin);
    
    // === КАЛІБРУВАННЯ (перші 20 ітерацій) ===
    if (calibrationCounter < CALIBRATION_ITERATIONS) {
        sensor2BaseLevel += sensor2;
        calibrationCounter++;
        
        if (calibrationCounter == CALIBRATION_ITERATIONS) {
            sensor2BaseLevel /= CALIBRATION_ITERATIONS;
            Serial.print("[КАЛІБРУВАННЯ] Базовий рівень датчика 2: ");
            Serial.println(sensor2BaseLevel);
        }
        
        sensor2PrevValue = sensor2;
        return; // Не працюємо під час калібрування
    }
    
    // Логування кожні 50 ітерацій
    static int logCounter = 0;
    if (++logCounter % 50 == 0) {
        Serial.print("[LADLE] S1=");
        Serial.print(sensor1);
        Serial.print(" | S2=");
        Serial.print(sensor2);
        Serial.print(" | Dist=");
        Serial.print(centerDistance);
        Serial.print("mm | Lifted=");
        Serial.println(isLifted ? "YES" : "NO");
    }
    
    // ========================================
    // ГОЛОВНА ЛОГІКА
    // ========================================
    
    // 1. Якщо відстань > 3 см - ОПУСКАЄМО
    if (centerDistance > CENTER_DROP_DISTANCE) {
        if (isLifted) {
            Serial.println("[LOGIC] Відстань > 3см - опускаємо");
            lowerLadle();
        }
        sensor2PrevValue = sensor2;
        return;
    }
    
    // 2. Якщо датчики "фонять", перевіряємо відстань
    // Якщо відстань > 3 см - ігноруємо датчики тиску
    
    // 3. Детекція натиску (різкий скачок або простий датчик)
    if (!isLifted) {
        if (detectSpikeAndPressure(sensor1, sensor2)) {
            // Перевіряємо відстань перед підйомом
            if (centerDistance <= CENTER_DROP_DISTANCE) {
                Serial.println("[LOGIC] НАТИСК ЗАСІЧЕНО + Відстань OK - ПІДЙОМ!");
                liftLadle();
            } else {
                Serial.println("[LOGIC] Натиск є, але відстань > 3см - ігноруємо (фон)");
            }
        }
    }
    
    // 4. Якщо підняли - тримаємо поки тиск є
    if (isLifted) {
        if (!isPressureStillPresent(sensor2)) {
            Serial.println("[LOGIC] Тиск зник - можна опускати при відстані > 3см");
            // Опускаємо тільки якщо відстань > 3см (пункт 1)
        }
    }
    
    sensor2PrevValue = sensor2;
}
