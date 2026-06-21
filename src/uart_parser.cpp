#include "uart_parser.h"

UARTParser::UARTParser(ServoDriver& driver) 
    : servo(driver), lastCommand(0) {}

void UARTParser::init() {
    Serial.begin(9600);
    // Ожидание инициализации UART
    delay(100);
    printMenu();
}

void UARTParser::process() {
    if (Serial.available()) {
        char cmd = Serial.read();
        // Игнорируем символы новой строки
        if (cmd == '\n' || cmd == '\r') {
            return;
        }
        cmd = toupper(cmd);
        if (validateCommand(cmd)) {
            processCommand(cmd);
            lastCommand = cmd;
        } else {
            Serial.print(F("❌ Неизвестная команда: '"));
            Serial.print(cmd);
            Serial.println(F("'"));
            Serial.println(F("Используйте M для меню или H для справки"));
        }
    }
}

bool UARTParser::validateCommand(char cmd) {
    return (cmd == '1' || cmd == '2' || cmd == '3' || 
            cmd == 'F' || cmd == 'R' || cmd == 'S' || 
            cmd == 'M' || cmd == 'H' || cmd == 'P' || 
            cmd == 'W' || cmd == '?');
}

void UARTParser::processCommand(char cmd) {
    switch(cmd) {
        case '1':
            executePulseCommand(1000);
            break;
        case '2':
            executePulseCommand(10000);
            break;
        case '3':
            executePulseCommand(50000);
            break;
        case 'F':
            servo.setDirection(true);
            break;
        case 'R':
            servo.setDirection(false);
            break;
        case 'S':
            servo.stopMotor();
            break;
        case 'M':
            printMenu();
            break;
        case 'H':
            printHelp();
            break;
        case 'P':
            servo.printStatus();
            break;
        case 'W': {
            // Команда для изменения ширины импульса
            Serial.println(F("Введите ширину импульса (2-1000 мкс):"));
            unsigned int newWidth = 0;
            while (!Serial.available()) {
                delay(10);
            }
            newWidth = Serial.parseInt();
            if (newWidth > 0) {
                servo.setPulseWidth(newWidth);
            } else {
                Serial.println(F("❌ Неверное значение!"));
            }
            break;
        }
        default:
            break;
    }
}

void UARTParser::executePulseCommand(unsigned long count) {
    if (!servo.isRunning()) {
        servo.generatePulses(count);
    } else {
        Serial.println(F("⚠ Двигатель уже работает!"));
    }
}

void UARTParser::printMenu() {
    Serial.println(F("╔════════════════════════════════════════════════╗"));
    Serial.println(F("║     ДИФФЕРЕНЦИАЛЬНЫЙ ВЫХОД ДЛЯ CN1           ║"));
    Serial.println(F("║           Arduino Nano ATmega328             ║"));
    Serial.println(F("╠════════════════════════════════════════════════╣"));
    Serial.println(F("║  Пины:                                       ║"));
    Serial.println(F("║    D5 = PULS+  (прямой)                      ║"));
    Serial.println(F("║    D3 = PULS-  (инверсный)                   ║"));
    Serial.println(F("║    D4 = SIGN+ (прямой)                       ║"));
    Serial.println(F("║    D2 = SIGN- (инверсный)                    ║"));
    Serial.println(F("╠════════════════════════════════════════════════╣"));
    Serial.println(F("║  Команды:                                    ║"));
    Serial.println(F("║    1 - 1000 импульсов (0.1 оборота)         ║"));
    Serial.println(F("║    2 - 10000 импульсов (1 оборот)           ║"));
    Serial.println(F("║    3 - 50000 импульсов (5 оборотов)         ║"));
    Serial.println(F("║    F - Вперед (CCW)                         ║"));
    Serial.println(F("║    R - Назад (CW)                           ║"));
    Serial.println(F("║    S - Стоп                                 ║"));
    Serial.println(F("║    P - Статус                               ║"));
    Serial.println(F("║    W - Ширина импульса                      ║"));
    Serial.println(F("║    M - Меню                                 ║"));
    Serial.println(F("║    H - Справка                              ║"));
    Serial.println(F("╚════════════════════════════════════════════════╝"));
}

void UARTParser::printHelp() {
    Serial.println(F("=== СПРАВКА ПО КОМАНДАМ ==="));
    Serial.println(F("1, 2, 3 - запуск двигателя на определенное количество импульсов"));
    Serial.println(F("  1 = 1000 импульсов (0.1 оборота)"));
    Serial.println(F("  2 = 10000 импульсов (1 оборот)"));
    Serial.println(F("  3 = 50000 импульсов (5 оборотов)"));
    Serial.println(F("F - установка направления ВПЕРЕД (CCW)"));
    Serial.println(F("R - установка направления НАЗАД (CW)"));
    Serial.println(F("S - экстренная остановка двигателя"));
    Serial.println(F("P - показать текущий статус"));
    Serial.println(F("W - изменить ширину импульса (2-1000 мкс)"));
    Serial.println(F("M - показать меню"));
    Serial.println(F("H или ? - показать эту справку"));
    Serial.println(F("==========================="));
    Serial.println(F(""));
    Serial.println(F("Примечания:"));
    Serial.println(F("• При работе двигателя можно остановить командой S"));
    Serial.println(F("• Ширина импульса по умолчанию: 10 мкс"));
    Serial.println(F("• Для точной настройки используйте команду W"));
    Serial.println(F("==========================="));
}