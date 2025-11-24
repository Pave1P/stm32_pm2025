#include <stdint.h>
#include "stm32f10x.h"

void delay_ms(uint32_t ms) {
    for (uint32_t i = 0; i < ms * 1000; i++) {
        __NOP();
    }
}

int main(void) {
    // Включаем тактирование порта C
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;

    // Настройка PC13 как выход
    GPIOC->CRH &= ~(GPIO_CRH_CNF13 | GPIO_CRH_MODE13);
    GPIOC->CRH |= GPIO_CRH_MODE13_0;

    // Настройка PC14 как вход с подтяжкой
    GPIOC->CRH &= ~(GPIO_CRH_CNF14 | GPIO_CRH_MODE14);
    GPIOC->CRH |= GPIO_CRH_CNF14_1;
    GPIOC->ODR |= GPIO_ODR_ODR14;

    // Настройка PC15 как вход с подтяжкой
    GPIOC->CRH &= ~(GPIO_CRH_CNF15 | GPIO_CRH_MODE15);
    GPIOC->CRH |= GPIO_CRH_CNF15_1;
    GPIOC->ODR |= GPIO_ODR_ODR15;

    uint32_t delay = 500000; // Начальная задержка (~1 Гц)
    const uint32_t min_delay = 7812;   // ~64 Гц
    const uint32_t max_delay = 500000; // ~1 Гц

    while (1) {
        // Кнопка A (PC14) - увеличиваем частоту
        if (!(GPIOC->IDR & GPIO_IDR_IDR14)) {
            if (delay > min_delay) {
                delay /= 2;
            }
            delay_ms(50);
            while (!(GPIOC->IDR & GPIO_IDR_IDR14));
            delay_ms(50);
        }

        // Кнопка C (PC15) - уменьшаем частоту
        if (!(GPIOC->IDR & GPIO_IDR_IDR15)) {
            if (delay < max_delay) {
                delay *= 2;
            }
            delay_ms(50);
            while (!(GPIOC->IDR & GPIO_IDR_IDR15));
            delay_ms(50);
        }

        // Мигаем светодиодом
        GPIOC->ODR ^= GPIO_ODR_ODR13;
        for (volatile uint32_t i = 0; i < delay; i++) { __NOP(); }
    }
}