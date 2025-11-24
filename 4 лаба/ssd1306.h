#ifndef SSD1306_H
#define SSD1306_H

#include <stdint.h>

// Определения пинов
#define SSD1306_CS_PIN      PIN_A0
#define SSD1306_DC_PIN      PIN_A1  
#define SSD1306_RESET_PIN   PIN_A2

// Размеры дисплея
#define SSD1306_WIDTH       128
#define SSD1306_HEIGHT      64
#define SSD1306_PAGES       8

// Команды дисплея
#define SSD1306_DISPLAYOFF          0xAE
#define SSD1306_DISPLAYON           0xAF
#define SSD1306_SETDISPLAYCLOCKDIV  0xD5
#define SSD1306_SETMULTIPLEX        0xA8
#define SSD1306_SETDISPLAYOFFSET    0xD3
#define SSD1306_SETSTARTLINE        0x40
#define SSD1306_CHARGEPUMP          0x8D
#define SSD1306_MEMORYMODE          0x20
#define SSD1306_SEGREMAP            0xA0
#define SSD1306_COMSCANDEC          0xC8
#define SSD1306_SETCOMPINS          0xDA
#define SSD1306_SETCONTRAST         0x81
#define SSD1306_SETPRECHARGE        0xD9
#define SSD1306_SETVCOMDETECT       0xDB
#define SSD1306_COLUMNADDR          0x21
#define SSD1306_PAGEADDR            0x22

// Прототипы функций
void SSD1306_Init(void);
void SSD1306_Command(uint8_t cmd);
void SSD1306_Data(uint8_t data);
void SSD1306_SetPosition(uint8_t x, uint8_t y);
void SSD1306_Clear(void);
void SSD1306_DrawChessboard(void);
void SSD1306_DrawTestPattern(void);
void SSD1306_Update(void);

#endif