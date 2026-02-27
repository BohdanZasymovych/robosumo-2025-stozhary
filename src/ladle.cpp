#include "ladle.h"

Ladle::Ladle(int s1Pin, int s2Pin, int fs1Pin, int fs2Pin)
    : servo1Pin(s1Pin),
      servo2Pin(s2Pin),
      forceSensor1Pin(fs1Pin),
      forceSensor2Pin(fs2Pin),
      currentAngle(LADLE_DOWN_ANGLE),
      isLifted(false),
      waitingForCloserDistance(false),
      lastLowerTime(0) {
}

void Ladle::init() {
    // ESP32Servo автоматично обере PWM канали 0-1 (мотори на 2-5)
    servo1.attach(servo1Pin);
    servo2.attach(servo2Pin);
    
    pinMode(forceSensor1Pin, INPUT);
    pinMode(forceSensor2Pin, INPUT);
    
    setAngle(LADLE_DOWN_ANGLE);
    
    Serial.println("\n=== LADLE INIT ===");
    Serial.println("Серваки: PWM канали 0-1 (авто)");
    Serial.println("2 датчики тиску: верхній >=170, нижній >=60");
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
    }
}

void Ladle::lowerLadle() {
    if (isLifted) {
        Serial.println("[LADLE] ОПУСКАННЯ");
        setAngle(LADLE_DOWN_ANGLE);
        isLifted = false;
    }
}

// Детекція різкого скачка та натиску
bool Ladle::detectSpikeAndPressure(int sensor1Val, int sensor2Val) {
    if (sensor1Val >= SENSOR1_PRESS_THRESHOLD) {
        Serial.print("[SENSOR1] Натиск! Val=");
        Serial.println(sensor1Val);
        return true;
    }

    if (sensor2Val >= SENSOR2_PRESS_THRESHOLD) {
        Serial.print("[SENSOR2] Натиск! Val=");
        Serial.println(sensor2Val);
        return true;
    }

    return false;
}

void Ladle::update(uint16_t centerDistance) {
    // Зчитуємо обидва датчики
    int sensor1 = analogRead(forceSensor1Pin);
    int sensor2 = analogRead(forceSensor2Pin);
    
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
        return;
    }

    // 2. Детекція натиску (верхній >=170 або нижній >=60)
    if (!isLifted) {
        if (detectSpikeAndPressure(sensor1, sensor2)) {
            Serial.println("[LOGIC] НАТИСК ЗАСІЧЕНО - ПІДЙОМ!");
            liftLadle();
        }
    }
}
