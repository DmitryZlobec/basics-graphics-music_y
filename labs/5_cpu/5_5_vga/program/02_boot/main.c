#include "memory_mapped_registers.h"
#include "seg7.h"

#include <coremark.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

// Определим номера выводов для индикаторов
#define LED_0   0x1
#define LED_1   0x2
#define LED_2   0x4
#define LED_3   0x8

#define digit_deleay 5000
#define letter_delay 5000

// Стандартный адрес загрузки для QEMU
#define BOOTLOADER_START_ADDRESS 0x80000000

// Текущий адрес загрузки
volatile uint32_t current_load_address = BOOTLOADER_START_ADDRESS;

#define LOAD_TIMEOUT_TICKS 27000000  // Приблизительно 5 секунд при 27 MHz
#define END_TIMEOUT_TICKS  1350000  // Приблизительно 5 секунд при 27 MHz



// Длина строки для преобразования ($initmemh)
#define HEX_STRING_LENGTH 8

// Структура описания команды
typedef struct CommandEntry {
    char command;          // Код команды
    void (*handler)(void); // Функция обработчик
} CommandEntry;

// Буфер для сбора HEX-данных
char hex_buffer[HEX_STRING_LENGTH+1]; // +1 для терминального нуля
size_t hex_idx = 0;



// Таблица команд
CommandEntry commands[] = {
    {'L', NULL}, // Команда начала загрузки (обработчик будет определен позже)
    {'R', NULL}, // Команда запуска программы (обработчик будет определен позже)
    {'T', NULL}, // Команда тестирования первых 16 слов,    
    {'M', NULL}, // Вывод другим способом
    {'W', NULL}, // Запись в памть

};

// Процедура задержки
void delay_nop(volatile int count) {
    while(count--) {
        asm("nop");
    }
}

// Очистка индикаторов
void clean() {
    port1 = 0x00;
    port0 = 0x00;
}

// Вывод буквы на индикаторы
void display_char(uint8_t led, uint8_t hex_code) {
    //To avoid blinking and shifting
    // First select HEX
    port1 = led;
    // Second put letter
    port0 = hex_code;
    
    // For boards DE0, DE0-CV with static hexs
    #ifdef STATIC
        clean();
    #endif
}


void H() {
    display_char(LED_3, HEX_H);
    delay_nop(digit_deleay);
}

void E() {
    display_char(LED_2, HEX_E);
    delay_nop(digit_deleay);
}

void L() {
    display_char(LED_1, HEX_L);
    delay_nop(digit_deleay);
}

void O() {
    display_char(LED_0, HEX_O);
    delay_nop(digit_deleay);
}

// Функция ручной конвертации HEX в 32-битное значение
uint32_t hex_to_uint32(const char *hex_str) {
    uint32_t result = 0;
    for(size_t i = 0; i < HEX_STRING_LENGTH; i += 2) {
        char high = hex_str[i];
        char low = hex_str[i+1];
        uint8_t nibble_high = (high >= 'A' ? high - 'A' + 10 : high - '0');
        uint8_t nibble_low = (low >= 'A' ? low - 'A' + 10 : low - '0');
        result |= ((nibble_high << 4 | nibble_low) << (28 - 4*i));
    }
    return result;
}

// Обработчик команды загрузки
void handle_load_command(void) {
    ee_printf(" Loading...\n");

    // Очистка буфера
    hex_idx = 0;
    hex_buffer[hex_idx] = '\0';
    // Подсчет загруженных байт
    volatile uint32_t loaded_bytes = 0;

    // Ожидание начала загрузки с таймаутом
    volatile unsigned long wait_ticks = 0;
    while(wait_ticks < LOAD_TIMEOUT_TICKS) {
        // Считываем значение из порта
        short raw_port_value = port7;

        // Проверяем доступность данных
        if(raw_port_value & (1 << 8)) {
            // Байты готовы, сохраняем первый байт
            char ch = raw_port_value & 0xFF;
            hex_buffer[hex_idx++] = ch;
            hex_buffer[hex_idx] = '\0';
            break; // Начало загрузки подтверждено
        }
        wait_ticks++;
    }

    if(wait_ticks >= LOAD_TIMEOUT_TICKS) {
        // Таймаут, загрузка не началась
        ee_printf(" Timeout waiting for data.\n");
        return;
    }

     // Главное тело загрузки
    while(true) {
        // Ждём следующую строку целиком
        volatile unsigned long ticks = 0;
        while(hex_idx < HEX_STRING_LENGTH && ticks < END_TIMEOUT_TICKS) {
            // Считываем значение из порта
            short raw_port_value = port7;

            // Проверяем наличие данных
            if(raw_port_value & (1 << 8)) {
                // Получили очередной байт
                char ch = raw_port_value & 0xFF;
                if( ch != '\n') {
                    hex_buffer[hex_idx++] = ch;
                    hex_buffer[hex_idx] = '\0';
                    ticks = 0; // Сброс таймера
                }
            } else {
                ticks++;
            }
        }

        if(ticks >= END_TIMEOUT_TICKS) {
            // Наступила большая пауза, завершаем загрузку
            break;
        }

        // Преобразуем строку и сохраняем
        uint32_t value = hex_to_uint32(hex_buffer);
        ee_printf("Loaded: 0x%X ",value);
        loaded_bytes += 4;
        break;
        // Готовимся к следующей строке
        hex_idx = 0;
        hex_buffer[hex_idx] = '\0';
    }

    // Итоговая статистика загрузки
    ee_printf(" Total loaded bytes: %u\n", loaded_bytes);
}



// Обработчик команды запуска
void handle_run_command(void) {
    // Передаем управление загруженной программе
    ((void (*)())current_load_address)();
}

// Обработчик команды загрузки
void handle_memtest_command(void) {
   
}


// Обработчик команды загрузки
void handle_memwr_command(void) {
   
}

// Обработчик команды тестирования (первых 16 слов)
void handle_test_command(void) {
    // Выводим первые 16 слов, начиная с адреса загрузки
    ee_printf("Test\n");

}

// Инициализация таблицы команд
void init_commands(void) {
    commands[0].handler = handle_load_command;
    commands[1].handler = handle_run_command;
    commands[2].handler = handle_test_command;
    commands[3].handler = handle_memtest_command;
    commands[4].handler = handle_memwr_command;
}

// Основная процедура загрузки
void bootload(void) {
    // Инициализация таблицы команд
    init_commands();

    // Бесконечный цикл обработки команд
    while(true) {
        // Проверяем наличие данных на порту
        short p7 = port7;
        if((p7 & (1 << 8)) == 0) {
            // Нет данных — отображаем HELLO
            H();E();L();O();
        } else {
            // Есть данные — получаем команду
            char ch = (port7 & 0xFF);

            // Поиск команды в таблице
            for(size_t i = 0; i < sizeof(commands)/sizeof(CommandEntry); i++) {
                if(commands[i].command == ch) {
                    // Выполняем соответствующий обработчик
                    commands[i].handler();
                    break;
                }
            }
        }
    }
}
extern  void cls();

extern  void cls1();
// Основной цикл программы
void main() {
    cls();
    ee_printf("\n YRV BootLoader v0.0.1 Send 'H' for help.\n");
    bootload(); // Запускаем загрузчик
    while(1) {} // Остаемся в цикле после завершения загрузки
}