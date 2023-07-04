#include "stm32f303xe.h"

int main(void) {
  // Enable GPIOA clock (p. 148 in the reference manual).
  RCC -> AHBENR |= 0x000A0000;
  // Configure GPIOA pin 5 as output
  // (by default it will then be push pull, see p. 237 of the reference manual).
  GPIOA -> MODER |= 0x00000400;
  // GPIOA pin 0 is connected to the motion sensor.
  // No need to configure it, because by default it's an input.

  while (1) {
    if (GPIOA -> IDR & 0x00000001) {
      GPIOA -> ODR |= 0x00000020;
    } else {
      GPIOA -> ODR &= ~0x00000020;
    }
  }
}
