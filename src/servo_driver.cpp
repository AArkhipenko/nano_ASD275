#include "servo_driver.h"

ServoDriver::ServoDriver() 
    : motorRunning(false), currentDirection(true), pulseWidth(10) {}

void ServoDriver::init() {
    // Настройка всех пинов как выходы
    pinMode(PULS_PLUS, OUTPUT);
    pinMode(PULS_MINUS, OUTPUT);
    pinMode(SIGN_PLUS, OUTPUT);
    pinMode(SIGN_MINUS, OUTPUT);
    
    // Начальное состояние
    setPulse(false);
    setSignals(true);  // Направление "Вперед"
    
    // Для Arduino Nano рекомендуется использовать более быстрые порты
    // но для совместимости оставляем стандартные digitalWrite
}

void ServoDriver::setPulse(bool state) {
    if (state) {
        digitalWrite(PULS_PLUS, HIGH);
        digitalWrite(PULS_MINUS, LOW);
    } else {
        digitalWrite(PULS_PLUS, LOW);
        digitalWrite(PULS_MINUS, HIGH);
    }
}

void ServoDriver::setSignals(bool forward) {
    currentDirection = forward;
    if (forward) {
        digitalWrite(SIGN_PLUS, HIGH);
        digitalWrite(SIGN_MINUS, LOW);
    } else {
        digitalWrite(SIGN_PLUS, LOW);
        digitalWrite(SIGN_MINUS, HIGH);
    }
}

void ServoDriver::generatePulses(unsigned long count) {
    if (motorRunning) {
        Serial.println(F("⚠ Двигатель уже работает! Дождитесь завершения."));
        return;
    }
    
    if (count == 0) {
        Serial.println(F("⚠ Количество импульсов должно быть больше 0!"));
        return;
    }
    
    motorRunning = true;
    
    Serial.print(F("▶ Генерация "));
    Serial.print(count);
    Serial.print(F(" импульсов = "));
    Serial.print((float)count / PULSES_PER_REVOLUTION, 2);
    Serial.println(F(" оборота"));
    
    Serial.print(F("  Направление: "));
    Serial.println(currentDirection ? F("ВПЕРЕД (CCW)") : F("НАЗАД (CW)"));
    Serial.print(F("  Ширина импульса: "));
    Serial.print(pulseWidth);
    Serial.println(F(" мкс"));
    
    unsigned long startTime = millis();
    
    // Оптимизация для Arduino Nano - используем локальные переменные
    uint8_t pulsPlusPin = PULS_PLUS;
    uint8_t pulsMinusPin = PULS_MINUS;
    
    // Предварительная проверка - можно ли генерировать с такой скоростью
    if (pulseWidth < 2) {
        Serial.println(F("⚠ Внимание: слишком маленькая ширина импульса!"));
    }
    
    for (unsigned long i = 0; i < count; i++) {
        // PULS+ = HIGH, PULS- = LOW
        digitalWrite(pulsPlusPin, HIGH);
        digitalWrite(pulsMinusPin, LOW);
        delayMicroseconds(pulseWidth);
        
        // PULS+ = LOW, PULS- = HIGH
        digitalWrite(pulsPlusPin, LOW);
        digitalWrite(pulsMinusPin, HIGH);
        delayMicroseconds(pulseWidth);
        
        // Индикация прогресса (каждые 1000 импульсов)
        if (i % 1000 == 0 && i > 0) {
            Serial.print(F("."));
            // Сброс сторожевого таймера для Arduino Nano (если используется)
            // wdt_reset();
        }
        
        // Проверка на STOP команду (можно добавить прерывание)
        if (Serial.available()) {
            char cmd = Serial.read();
            if (cmd == 'S' || cmd == 's') {
                Serial.println(F("\n⏹ Остановка по команде!"));
                motorRunning = false;
                setPulse(false);
                return;
            }
        }
    }
    
    // Возврат в исходное состояние
    setPulse(false);
    
    unsigned long elapsedTime = millis() - startTime;
    
    Serial.println();
    Serial.print(F("✅ Готово! Время выполнения: "));
    Serial.print(elapsedTime);
    Serial.println(F(" мс"));
    Serial.print(F("Средняя частота: "));
    Serial.print((float)count * 1000.0 / elapsedTime);
    Serial.println(F(" Гц"));
    Serial.println();
    
    motorRunning = false;
}

void ServoDriver::setDirection(bool forward) {
    if (motorRunning) {
        Serial.println(F("⚠ Нельзя менять направление во время движения!"));
        return;
    }
    
    setSignals(forward);
    Serial.print(F("▶ Направление: "));
    Serial.println(forward ? F("ВПЕРЕД (CCW)") : F("НАЗАД (CW)"));
}

void ServoDriver::stopMotor() {
    setPulse(false);
    motorRunning = false;
    Serial.println(F("⏹ СТОП"));
}

void ServoDriver::setPulseWidth(unsigned int width) {
    if (width < 2) {
        Serial.println(F("⚠ Минимальная ширина импульса: 2 мкс"));
        pulseWidth = 2;
    } else if (width > 1000) {
        Serial.println(F("⚠ Максимальная ширина импульса: 1000 мкс"));
        pulseWidth = 1000;
    } else {
        pulseWidth = width;
        Serial.print(F("▶ Ширина импульса установлена: "));
        Serial.print(pulseWidth);
        Serial.println(F(" мкс"));
    }
}

bool ServoDriver::isRunning() const {
    return motorRunning;
}

void ServoDriver::printStatus() const {
    Serial.println(F("=== СТАТУС СЕРВОДВИГАТЕЛЯ ==="));
    Serial.print(F("Состояние: "));
    Serial.println(motorRunning ? F("РАБОТАЕТ") : F("ОСТАНОВЛЕН"));
    Serial.print(F("Направление: "));
    Serial.println(currentDirection ? F("ВПЕРЕД (CCW)") : F("НАЗАД (CW)"));
    Serial.print(F("Ширина импульса: "));
    Serial.print(pulseWidth);
    Serial.println(F(" мкс"));
    Serial.println(F("=============================="));
}