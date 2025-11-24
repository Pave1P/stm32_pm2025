#include "ssd1306.h"
#include "spi.h"

// Буфер дисплея в RAM
uint8_t SSD1306_Buffer[SSD1306_WIDTH * SSD1306_PAGES];

void SSD1306_Init(void) {
    // Инициализация пинов
    TRISAbits.TRISA0 = 0;  // CS как выход
    TRISAbits.TRISA1 = 0;  // DC как выход
    TRISAbits.TRISA2 = 0;  // RESET как выход
    
    // Аппаратный сброс
    PORTAbits.RA2 = 0;     // RESET = 0
    __delay_ms(10);
    PORTAbits.RA2 = 1;     // RESET = 1
    __delay_ms(10);
    
    // Инициализация SPI
    SPI1_Init();
    
    // Последовательность инициализации дисплея
    SSD1306_Command(SSD1306_DISPLAYOFF);                    // 0xAE
    
    SSD1306_Command(SSD1306_SETDISPLAYCLOCKDIV);           // 0xD5
    SSD1306_Command(0x80);                                 // suggested ratio
    
    SSD1306_Command(SSD1306_SETMULTIPLEX);                 // 0xA8
    SSD1306_Command(0x3F);                                 // 64-1
    
    SSD1306_Command(SSD1306_SETDISPLAYOFFSET);             // 0xD3
    SSD1306_Command(0x00);                                 // no offset
    
    SSD1306_Command(SSD1306_SETSTARTLINE | 0x00);          // 0x40 line #0
    
    SSD1306_Command(SSD1306_CHARGEPUMP);                   // 0x8D
    SSD1306_Command(0x14);                                 // enable charge pump
    
    SSD1306_Command(SSD1306_MEMORYMODE);                   // 0x20
    SSD1306_Command(0x00);                                 // horizontal addressing
    
    SSD1306_Command(SSD1306_SEGREMAP | 0x01);              // 0xA0 remap
    SSD1306_Command(SSD1306_COMSCANDEC);                   // 0xC8
    
    SSD1306_Command(SSD1306_SETCOMPINS);                   // 0xDA
    SSD1306_Command(0x12);                                 // sequential, disable remap
    
    SSD1306_Command(SSD1306_SETCONTRAST);                  // 0x81
    SSD1306_Command(0xCF);                                 // contrast value
    
    SSD1306_Command(SSD1306_SETPRECHARGE);                 // 0xD9
    SSD1306_Command(0xF1);                                 // precharge period
    
    SSD1306_Command(SSD1306_SETVCOMDETECT);                // 0xDB
    SSD1306_Command(0x40);                                 // vcomh level
    
    SSD1306_Command(SSD1306_DISPLAYON);                    // 0xAF
    
    // Очистка дисплея
    SSD1306_Clear();
}

void SSD1306_Command(uint8_t cmd) {
    PORTAbits.RA1 = 0;     // DC = 0 (команда)
    PORTAbits.RA0 = 0;     // CS = 0 (активен)
    
    SPI1_Write(cmd);
    
    PORTAbits.RA0 = 1;     // CS = 1 (неактивен)
}

void SSD1306_Data(uint8_t data) {
    PORTAbits.RA1 = 1;     // DC = 1 (данные)
    PORTAbits.RA0 = 0;     // CS = 0 (активен)
    
    SPI1_Write(data);
    
    PORTAbits.RA0 = 1;     // CS = 1 (неактивен)
}

void SSD1306_SetPosition(uint8_t x, uint8_t y) {
    SSD1306_Command(SSD1306_COLUMNADDR);   // 0x21
    SSD1306_Command(x);                    // start column
    SSD1306_Command(SSD1306_WIDTH - 1);    // end column
    
    SSD1306_Command(SSD1306_PAGEADDR);     // 0x22
    SSD1306_Command(y);                    // start page
    SSD1306_Command(SSD1306_PAGES - 1);    // end page
}

void SSD1306_Clear(void) {
    // Заполнение буфера нулями
    for(uint16_t i = 0; i < sizeof(SSD1306_Buffer); i++) {
        SSD1306_Buffer[i] = 0x00;
    }
    SSD1306_Update();
}

void SSD1306_DrawChessboard(void) {
    uint8_t pattern1 = 0xAA;  // 10101010
    uint8_t pattern2 = 0x55;  // 01010101
    
    for(uint16_t i = 0; i < sizeof(SSD1306_Buffer); i++) {
        // Чередование паттернов для шахматной доски
        if((i / 8) % 2 == 0) {
            SSD1306_Buffer[i] = (i % 2) ? pattern1 : pattern2;
        } else {
            SSD1306_Buffer[i] = (i % 2) ? pattern2 : pattern1;
        }
    }
    SSD1306_Update();
}

void SSD1306_DrawTestPattern(void) {
    // Очистка буфера
    for(uint16_t i = 0; i < sizeof(SSD1306_Buffer); i++) {
        SSD1306_Buffer[i] = 0x00;
    }
    
    // Рисование рамки
    for(uint8_t x = 0; x < SSD1306_WIDTH; x++) {
        // Верхняя и нижняя границы
        if(x < SSD1306_PAGES) {
            SSD1306_Buffer[x] |= 0x01;                    // Верх
            SSD1306_Buffer[(SSD1306_HEIGHT/8-1)*SSD1306_WIDTH + x] |= 0x80; // Низ
        }
    }
    
    for(uint8_t y = 0; y < SSD1306_PAGES; y++) {
        // Левая и правая границы
        SSD1306_Buffer[y * SSD1306_WIDTH] |= 0xFF;        // Левая
        SSD1306_Buffer[y * SSD1306_WIDTH + SSD1306_WIDTH - 1] |= 0xFF; // Правая
    }
    
    // Рисование диагоналей
    for(uint8_t i = 0; i < SSD1306_WIDTH && i < SSD1306_PAGES * 8; i++) {
        uint8_t page = i / 8;
        uint8_t bit = 1 << (i % 8);
        SSD1306_Buffer[page * SSD1306_WIDTH + i] |= bit;          // Главная диагональ
        SSD1306_Buffer[page * SSD1306_WIDTH + (SSD1306_WIDTH - 1 - i)] |= bit; // Побочная диагональ
    }
    
    SSD1306_Update();
}

void SSD1306_Update(void) {
    SSD1306_SetPosition(0, 0);
    
    // Отправка всего буфера в дисплей
    for(uint16_t i = 0; i < sizeof(SSD1306_Buffer); i++) {
        SSD1306_Data(SSD1306_Buffer[i]);
    }
}