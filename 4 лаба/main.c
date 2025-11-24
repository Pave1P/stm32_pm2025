#include <xc.h>
#include "ssd1306.h"

// Конфигурация PIC16F877A
#pragma config FOSC = HS   // High Speed oscillator
#pragma config WDTE = OFF  // Watchdog Timer disabled
#pragma config PWRTE = OFF // Power-up Timer disabled
#pragma config BOREN = ON  // Brown-out Reset enabled
#pragma config LVP = OFF   // Low Voltage Programming disabled
#pragma config CPD = OFF   // Data EEPROM Memory Code Protection off
#pragma config WRT = OFF   // Flash Program Memory Write Enable bits off
#pragma config CP = OFF    // Flash Program Memory Code Protection bit off

#define _XTAL_FREQ 20000000 // 20 MHz

void main(void) {
    // Настройка портов
    TRISD = 0x00;          // PORTD как выходы для отладки
    PORTD = 0x00;
    
    // Инициализация дисплея
    SSD1306_Init();
    __delay_ms(100);
    
    while(1) {
        // Тест 1: Шахматная доска
        SSD1306_DrawChessboard();
        __delay_ms(2000);
        
        // Тест 2: Тестовый паттерн с рамкой
        SSD1306_DrawTestPattern();
        __delay_ms(2000);
        
        // Тест 3: Очистка экрана
        SSD1306_Clear();
        __delay_ms(1000);
        
        // Индикация на PORTD
        PORTD++;
    }
}