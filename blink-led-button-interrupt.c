#include "main.h"
#include "../Inc/stm32f3xx_hal.h"

typedef enum {
  FALSE,
  TRUE
} BOOL;

void EXTI15_10_IRQHandler(void) {
  EXTI -> PR |= 0x00002000; // Reset the interrupt request flag.
  GPIOA -> ODR ^= 0x00000020; // Toggle GPIOA pin 5 (LED)
}

int main(void) {
  RCC -> AHBENR |= 0x000A0000; // Enable GPIOA and GPIOC clocks.

  RCC -> APB2ENR |= 0x00000001; // Enable the SYSCFG clock. This is necessary for connecting PC13 to EXTI13.

  GPIOA -> MODER |= 0x00000400; // Configure GPIOA pin 5 as output.

  // PA13 is the push button. No need to configure it, because by default it's an input.

  GPIOA -> OTYPER &= ~0x00000020; // (1 << 5) Configure GPIOA pin 5 as push-pull.

  // Configure EXTI13(extended interrupt), to which PC13 can be connected:
  // Connect PC13 to EXTI13. See p. 252 of the reference manual.
  SYSCFG -> EXTICR[3] |= 0x00000020; // EXTICR4 = EXTICR[3]

  EXTI -> FTSR |= 0x00002000; // Set EXTI13 to be triggered by a falling edge.

  EXTI -> IMR |= 0x00002000; // Enable EXTI13.

  NVIC_EnableIRQ(EXTI15_10_IRQn); // Enable the interrupt handler for EXTI13.

  while (1) {
    // All the functionality of this program is implemented in the IRQ handler.
  }
}
