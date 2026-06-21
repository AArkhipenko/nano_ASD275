#ifndef UART_PARSER_H
#define UART_PARSER_H

#include <Arduino.h>
#include "servo_driver.h"

class UARTParser {
public:
    UARTParser(ServoDriver& driver);
    void init();                          // Инициализация UART
    void process();                       // Обработка входящих команд
    void printMenu();                     // Вывод меню
    void printHelp();                     // Вывод справки
    
private:
    ServoDriver& servo;
    char lastCommand;                     // Последняя выполненная команда
    
    void processCommand(char cmd);        // Обработка конкретной команды
    void executePulseCommand(unsigned long count);  // Выполнение команды импульсов
    bool validateCommand(char cmd);       // Проверка валидности команды
};

#endif // UART_PARSER_H