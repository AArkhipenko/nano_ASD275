/*
  Управление сервоприводом ASD через CN1 (ДИФФЕРЕНЦИАЛЬНЫЙ ВЫХОД)
  Платформа: Arduino Nano ATmega328
  
  Подключение:
  D5 (PULS+)  → CN1 пин 5  (PULS+)
  D3 (PULS-)  → CN1 пин 21 (PULS-)  ← ИНВЕРСНЫЙ СИГНАЛ!
  D4 (SIGN+)  → CN1 пин 6  (SIGN+)
  D2 (SIGN-)  → CN1 пин 22 (SIGN-)  ← ИНВЕРСНЫЙ СИГНАЛ!
  
  ВНИМАНИЕ: PULS- и SIGN- - это инверсные сигналы!
  Для Arduino Nano используйте пины D2-D13
*/

#include <Arduino.h>
#include "servo_driver.h"
#include "uart_parser.h"

// Создание глобальных объектов
ServoDriver servo;
UARTParser parser(servo);

void setup() {
    // Инициализация драйвера сервопривода
    servo.init();
    
    // Инициализация UART парсера
    parser.init();
    
    // Информация о системе
    Serial.println(F("Система инициализирована!"));
    Serial.print(F("Платформа: Arduino Nano ATmega328"));
    Serial.println(F(" @ 16 MHz"));
    Serial.println();
    
    // Небольшая задержка для стабильности
    delay(100);
}

void loop() {
    // Обработка команд UART
    parser.process();
    
    // Добавляем небольшую задержку для снижения нагрузки на процессор
    // и предотвращения зависаний на Arduino Nano
    delay(1);
}

// Обработчик прерываний для аварийной остановки (опционально)
// Можно подключить к кнопке или внешнему сигналу
// ISR(INT0_vect) {
//     servo.stopMotor();
// }