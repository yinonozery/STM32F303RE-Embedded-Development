#include "main.h"
#include "../Inc/stm32f3xx_hal.h"

int main(void) {
	HAL_Init();

	RCC->AHBENR |= 0x00020000; // Enable GPIOA clock (p. 148 in the datasheet)
	
	GPIOA->MODER |= 0x00000400; // Configure GPIOA pin 5 as output.
	
	GPIOA->OTYPER &= ~0x00000020; // (1 << 5) Configure GPIOA pin 5 as push-pull.

	RCC->APB1ENR |= 0x00000001; // Enable TIM2

	TIM2->ARR = 4000000; //0x3D0900; // Set ARR timer count to 4,000,000 (half second)

	TIM2->CR1 |= TIM_CR1_CEN; // 0x00000001, Enable counter by set CEN bit to 1

	/* Infinite loop */
	while (1) {
		if (TIM2->SR & 1) { // Check if clock reached 4,000,000 ticks
			GPIOA->ODR ^= 0x00000020; // Toggle GPIOA pin 5 (LED)
			TIM2->SR &= ~0x00000001; // Reset counter bit flag
		}
	}
}
