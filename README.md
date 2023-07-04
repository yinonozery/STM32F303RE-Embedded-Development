##  Embedded Projects with STM32F303RE on Nucleo-64 Board
<div align="center">
  <img src="https://github.com/yinonozery/STM32F303RE-Embedded-Development/assets/74764366/05ac8402-67e7-45a6-8d7a-5bb8f39caa1c" alt="STM32F303RE" width="450" />
</div>

This repository contains a collection of embedded projects developed using the STM32F303RE microcontroller on the Nucleo-64 development board. Each project showcases various functionalities and features of the microcontroller in conjunction with other onboard components.
The projects included are:

- **LED Blinking with Timer**: Blinks an LED using timer (TIM2).
  
- **LED Blinking with Timer Interrupt**: Blinks an LED at a specific interval using timer (TIM2) interrupts.

- **LED Blinking with Button Interrupt**: Blinks an LED using timer interrupts and interrupts of a button press.

- **Counting Time Between Clicks**: Measures and displays the time interval between button clicks using timer interrupts.

- **USART2**: USART2 library for serial communication.

- **LED Modes with Buffer**: Allows switching LED modes ('on', 'blink', 'off') based on user input received through USART2 communication.

- **LED Modes with Cyclic Buffer**: Implements LED modes ('on', 'blink', 'off') using a cyclic buffer for efficient handling of user input.

- **Voltage Detector**: Detects voltage levels using GPIO pins and provides corresponding output messages.

- **External Motion Sensor**: Interfacing with an external motion sensor to detect motion and trigger LED toggle.
