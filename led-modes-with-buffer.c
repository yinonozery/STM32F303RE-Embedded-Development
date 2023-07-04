#include "main.h"
#include "../Inc/stm32f3xx_hal.h"
#include <string.h>
#include "usart2.h"

#define INPUT_BUFFER_SIZE 10

//	Modes: 'on' | 'blink' | 'off'

static char buffer[INPUT_BUFFER_SIZE];
static int curr_index = 0; // Keep current writing index
static int flag = 0;

void TIM2_IRQHandler(void) {
  GPIOA -> ODR ^= 0x00000020; // Toggle GPIOA pin 5 (LED)
  TIM2 -> SR &= ~0x00000001; // Reset counter bit flag
}

void add_char_to_buffer() {
  char tmp = (char) USART2 -> RDR;
  buffer[curr_index] = tmp;
  curr_index++;
  if (USART2 -> RDR == 13) // Check if user pressed Enter
    flag = 1;
}

void handle_input(char * temp_buffer) {
  if (strncmp(temp_buffer, "on\r", 3) == 0) {
    TIM2 -> CR1 &= ~TIM_CR1_CEN; // Disable timer
    GPIOA -> ODR |= 0x00000020; // Turn on LED
  } else if (strcmp(temp_buffer, "blink\r") == 0) {
    TIM2 -> CR1 |= TIM_CR1_CEN; // Enable timer
  } else if ((strncmp(temp_buffer, "off\r", 4) == 0)) {
    TIM2 -> CR1 &= ~TIM_CR1_CEN; // Disable timer
    GPIOA -> ODR &= ~0x00000020; // Turn off LED
  }
}

int main(void) {
  HAL_Init();
  USART2_init();

  GPIOA -> MODER |= 0x00000400; // Configure GPIOA pin 5 as output (LED)
  RCC -> APB1ENR |= 0x00000001; // Enable TIM2 clock (p. 67 in the datasheet)
  TIM2 -> ARR = 2000000; // Set ARR timer count to 2,000,000 (0.25 second)
  TIM2 -> DIER |= TIM_SR_UIF; // 0x00000001
  NVIC_EnableIRQ(TIM2_IRQn); // Enable the timer interrupt in the core

  while (1) {
    USART2_getCharacter(); // Infinite loop till user enter a character
    add_char_to_buffer();

    if (flag) {
      char temp_buffer[INPUT_BUFFER_SIZE];
      strcpy(temp_buffer, buffer); // Copy to a temporary buffer
      memset(buffer, 0, sizeof(buffer)); // Clear buffer
      curr_index = 0;
      flag = 0;
      handle_input(temp_buffer);
    }
  };
}
