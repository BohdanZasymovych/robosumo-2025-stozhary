#include "ladle.h"

Ladle::Ladle(int s1Pin, int s2Pin, int forceSensorPin)
    : servo1Pin(s1Pin),
      servo2Pin(s2Pin),
      forceSensorPin(forceSensorPin),
      currentAngle(LADLE_DOWN_ANGLE),
      isLifted(false),
      waitingForCloserDistance(false),
      forceBufferIndex(0),
      lastLowerTime(0) {
    // Ініціалізація буфера
    for (int i = 0; i < FORCE_FILTER_SIZE; i++) {
        forceBuffer[i] = 0;
    }
}

void Ladle::init() {
    // Підключаємо серво
    servo1.attach(servo1Pin);
    servo2.attach(servo2Pin);
    
    // Ініціалізуємо датчик сили
    pinMode(forceSensorPin, INPUT);
    
    // Опускаємо ківш на початок
    setAngle(LADLE_DOWN_ANGLE);
    
    Serial.println("\n=== LADLE INIT ===");
    Serial.println("Режим: автоматичне керування на основі датчиків");
}

int Ladle::getFilteredForce(int newValue) {
    forceBuffer[forceBufferIndex] = newValue;
    forceBufferIndex = (forceBufferIndex + 1) % FORCE_FILTER_SIZE;
    
    int sum = 0;
    for (int i = 0; i < FORCE_FILTER_SIZE; i++) {
        sum += forceBuffer[i];
    }
    return sum / FORCE_FILTER_SIZE;
}

void Ladle::setAngle(int angle) {
    currentAngle = angle;
    servo1.write(currentAngle);
    servo2.write(180 - currentAngle);
}

void Ladle::liftLadle() {
    if (!isLifted) {
        Serial.println("[LADLE] Підйом на 60°");
        setAngle(LADLE_LIFT_ANGLE);
        isLifted = true;
        waitingForCloserDistance = false;
    }
}

void Ladle::lowerLadle() {
    if (isLifted) {
        Serial.println("[LADLE] Опускання");
        setAngle(LADLE_DOWN_ANGLE);
        isLifted = false;
        waitingForCloserDistance = false;
    }
}

void Ladle::update(uint16_t centerDistance) {
    // Зчитуємо та фільтруємо датчик сили
    int rawForce = analogRead(forceSensorPin);
    int filteredForce = getFilteredForce(rawForce);
    
    // Логування (кожне 50-е значення)
    static int logCounter = 0;
    if (++logCounter % 50 == 0) {
        Serial.print("[LADLE] Force=");
        Serial.print(filteredForce);
        Serial.print(" | Center=");
        Serial.print(centerDistance);
        Serial.print("mm | Angle=");
        Serial.print(currentAngle);
        Serial.print("° | Lifted=");
        Serial.println(isLifted ? "YES" : "NO");
    }
    
    // ========================================
    // ГОЛОВНА ЛОГІКА
    // ========================================
    
    // 1. Якщо центральний датчик показує > 3 см - ОПУСКАЄМО
    if (centerDistance > CENTER_DROP_DISTANCE) {
        if (isLifted) {
            Serial.println("[LADLE] Центральний > 3см - опускаємо");
            lowerLadle();
        }
        return;
    }
    
    // 2. Датчик сили спрацював
    if (filteredForce > FORCE_THRESHOLD) {
        // Особливий випадок: якщо центральний датчик > 2 см
        if (centerDistance > MIDDLE_WAIT_DISTANCE) {
            // Чекаємо поки центральний датчик не покаже 0.5 см
            if (!waitingForCloserDistance) {
                Serial.println("[LADLE] Сила спрацювала, але центральний > 2см - очікування");
                waitingForCloserDistance = true;
            }
            
            // Перевіряємо чи центральний датчик показує вже 0.5 см
            if (centerDistance <= MIDDLE_LIFT_DISTANCE) {
                Serial.println("[LADLE] Центральний <= 0.5см - ПІДЙОМ");
                liftLadle();
            }
            return;
        }
        
        // Звичайний випадок - просто піднімаємо
        if (!isLifted) {
            Serial.println("[LADLE] Датчик сили спрацював - ПІДЙОМ");
            liftLadle();
        }
        return;
    }
    
    // 3. Центральний датчик показує 0.5 см або менше
    if (centerDistance <= MIDDLE_LIFT_DISTANCE) {
        if (!isLifted) {
            Serial.println("[LADLE] Центральний <= 0.5см - ПІДЙОМ");
            liftLadle();
        }
        return;
    }
    
    // 4. Якщо чекали на зближення, але сила вже не спрацьовує
    if (waitingForCloserDistance && filteredForce <= FORCE_THRESHOLD) {
        Serial.println("[LADLE] Очікування скасовано (сила зникла)");
        waitingForCloserDistance = false;
    }
}
