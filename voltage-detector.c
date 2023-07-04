#include "stm32f303xe.h"
#include "usart2.h"

void EXTI0_IRQHandler() {
  // Clear the interrupt pending flag.
  EXTI -> PR |= EXTI_PR_PR0;
  // Check the value of PA0.
  if (GPIOA -> IDR & GPIO_IDR_0) {
    // PA0 is high (jumper connected to 3.3V).
    USART2_print("hot\n");
  } else if (!(GPIOA -> IDR & GPIO_IDR_0)) {
    // PA0 is low (jumper connected to GND).
    USART2_print("cold!\n");
  }
}

int main(void) {
  // Enable GPIOA, SYSCFG and USART2 clocks.
  RCC -> AHBENR |= 0x00020000;
  RCC -> APB1ENR |= 0x00020000;
  RCC -> APB2ENR |= 0x00000001;
  SYSCFG -> EXTICR[0] &= ~0x00000001;

  // Configure GPIOA pins 2 and 3 as alternate function mode (AF mode) for USART2.
  GPIOA -> MODER |= 0x000000A0; // Set PA2 and PA3 to AF mode.
  GPIOA -> AFR[0] |= 0x00007700; // Set PA2 & PA3 to AF7 (USART2_TX, USART2_RX).

  // Configure USART2 for 9600 baud, 8 data bits, no parity, 1 stop bit.
  USART2 -> BRR = 833; // Baud rate = 9600.
  USART2 -> CR1 |= 0x000D; // D = 1101, 11 = Enable transmit and receive, 01 = Enable USART2

  // Configure PA0 as input with pull-down.
  GPIOA -> MODER &= ~GPIO_MODER_MODER0_Msk;
  GPIOA -> PUPDR &= ~GPIO_PUPDR_PUPDR0_Msk;
  GPIOA -> PUPDR |= GPIO_PUPDR_PUPDR0_1;

  // Enable EXTI0 interrupt on falling and rising edges.
  EXTI -> RTSR |= EXTI_RTSR_TR0;
  EXTI -> FTSR |= EXTI_FTSR_TR0;
  EXTI -> IMR |= EXTI_IMR_MR0;

  // Enable the interrupt in NVIC.
  NVIC_EnableIRQ(EXTI0_IRQn);

  while (1) {};
}
