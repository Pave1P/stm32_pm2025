#include "spi.h"
#include <xc.h>

void SPI1_Init(void) {
    // Настройка пинов SPI
    TRISBbits.TRISB6 = 0;  // SCK как выход
    TRISBbits.TRISB5 = 0;  // SDO как выход
    TRISBbits.TRISB4 = 1;  // SDI как вход (не используется)
    
    // Настройка SPI1
    SSP1STAT = 0x40;       // CKE = 1, SMP = 0
    SSP1CON1 = 0x32;       // SPI Master, clock = FOSC/64, CKP = 1
}

void SPI1_Write(uint8_t data) {
    SSP1BUF = data;        // Запись данных в буфер
    while(!SSP1STATbits.BF); // Ожидание завершения передачи
}

uint8_t SPI1_Read(void) {
    SSP1BUF = 0x00;        // Запуск передачи для чтения
    while(!SSP1STATbits.BF); // Ожидание приема данных
    return SSP1BUF;        // Возврат принятых данных
}