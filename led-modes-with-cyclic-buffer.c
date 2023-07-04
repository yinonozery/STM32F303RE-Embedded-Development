#include "main.h"
#include "../Inc/stm32f3xx_hal.h"
#include "usart2.h"
#include <string.h>

#define INPUT_BUFFER_SIZE 10

//	Modes: 'on' | 'blink' | 'off'

static char buffer[INPUT_BUFFER_SIZE];
static int readIndex = 0;
static int writeIndex = 0;
static int flag = 0;
static int BufferFullFlag = 0;

void TIM2_IRQHandler(void) {
  GPIOA -> ODR ^= 0x00000020; // Toggle GPIOA pin 5 (LED)
  TIM2 -> SR &= ~0x00000001; // Reset counter bit flag
}

void USART2_IRQHandler() {
  USART2 -> ISR |= 0x00000020;
  char readChar = (char) USART2 -> RDR;
  buffer[writeIndex] = readChar;
  writeIndex++;
  if (writeIndex == INPUT_BUFFER_SIZE) {
    BufferFullFlag = 1;
    writeIndex = 0;
  }

  //	print("Write %d, Read %d, Char %c (ASCII %d)\n", writeIndex, readIndex, readChar, (int) readChar);

  if (BufferFullFlag && readChar == 13) {
    int TEMP_BUFFER_SIZE = INPUT_BUFFER_SIZE - readIndex + writeIndex;
    char temp_buffer[TEMP_BUFFER_SIZE];
    for (int i = 0, j = 0; i < TEMP_BUFFER_SIZE; i++, j++) {
      if (readIndex >= INPUT_BUFFER_SIZE)
        readIndex = 0;
      temp_buffer[j] = buffer[readIndex];
      readIndex++;
    }
    readIndex = writeIndex;
    BufferFullFlag = 0;
    handle_input(temp_buffer);
  } else if (readChar == 13) { // Check if user input new-line '\n'
    flag = 1;
  }
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

  USART2 -> CR1 |= 0x00000020; // Set reading interrupt
  GPIOA -> MODER |= 0x00000400; // Configure GPIOA pin 5 as output (LED)
  RCC -> APB1ENR |= 0x00000001; // Enable TIM2 clock (p. 67 in the datasheet)
  TIM2 -> ARR = 2000000; // Set ARR timer count to 2,000,000 (0.25 second)
  TIM2 -> DIER |= TIM_SR_UIF; // 0x00000001
  NVIC_EnableIRQ(TIM2_IRQn); // Enable the timer interrupt in the core
  NVIC_EnableIRQ(USART2_IRQn); // Enable the USART2 read interrupt

  while (1) {
    if (flag) {
      char temp_buffer[writeIndex - readIndex];
      strncpy(temp_buffer, buffer + readIndex, writeIndex - readIndex); // Copy to a temporary buffer
      readIndex = writeIndex;
      flag = 0;
      handle_input(temp_buffer);
    }
  };
}
