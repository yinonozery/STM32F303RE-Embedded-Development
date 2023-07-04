#include "stm32f303xe.h"
#include "usart2.h"

typedef enum {
  FALSE,
  TRUE
} BOOL;

uint32_t timer = 0;
BOOL button_pressed = FALSE;

void EXTI15_10_IRQHandler() {
  EXTI -> PR |= 0x00002000;
  button_pressed = TRUE;
}

void TIM2_IRQHandler() {
  TIM2 -> SR &= ~0x00000001;
  timer++;
}

void TIM2_init() {
  RCC -> APB1ENR |= 0x00000001;
  TIM2 -> ARR = 8000000;
  TIM2 -> DIER |= 0x00000001;
  TIM2 -> CR1 |= 0x00000001;
  NVIC_EnableIRQ(TIM2_IRQn);
}

void BUTTON_init() {
  RCC -> AHBENR |= 0x00080000; // PORT C
  RCC -> APB2ENR |= 0x00000001; // SYSCFG
  GPIOC -> MODER &= ~0x01000000; // INPUT
  GPIOC -> OTYPER &= ~0x01000000; // PUSH-PULL
  SYSCFG -> EXTICR[3] |= 0x00000020;
  EXTI -> FTSR |= 0x00002000;
  EXTI -> IMR |= 0x00002000;
  NVIC_EnableIRQ(EXTI15_10_IRQn);
}

int main(void) {
  HAL_Init();
  USART2_init();
  TIM2_init();
  BUTTON_init();
  print("App starting...\n");

  while (1) {
    if (button_pressed) {
      print("Time between clicks %d\n", timer);
      button_pressed = FALSE;
      timer = 0;
    }
  }
}
