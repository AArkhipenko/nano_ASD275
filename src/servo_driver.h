#ifndef SERVO_DRIVER_H
#define SERVO_DRIVER_H

#include <Arduino.h>

// Определение пинов для Arduino Nano
// Используем цифровые пины D2-D13
#define PULS_PLUS  5   // D5 - PULS+ (прямой сигнал)
#define PULS_MINUS 3   // D3 - PULS- (инверсный сигнал)
#define SIGN_PLUS  4   // D4 - SIGN+ (прямой сигнал)
#define SIGN_MINUS 2   // D2 - SIGN- (инверсный сигнал)

// Константы для расчета оборотов
#define PULSES_PER_REVOLUTION 10000  // 10000 импульсов = 1 оборот

class ServoDriver {
public:
    ServoDriver();
    void init();                          // Инициализация пинов
    void generatePulses(unsigned long count);  // Генерация импульсов
    void setDirection(bool forward);      // Установка направления
    void stopMotor();                     // Остановка двигателя
    bool isRunning() const;               // Проверка состояния
    void printStatus() const;             // Вывод статуса
    void setPulseWidth(unsigned int width); // Установка ширины импульса (мкс)
    
private:
    bool motorRunning;                    // Флаг работы двигателя
    bool currentDirection;                // Текущее направление (true = вперед)
    unsigned int pulseWidth;              // Ширина импульса в микросекундах
    
    void setPulse(bool state);            // Установка состояния импульса
    void setSignals(bool forward);        // Установка сигналов направления
};

#endif // SERVO_DRIVER_H