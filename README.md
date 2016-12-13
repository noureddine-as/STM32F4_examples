# STM32F4_examples
Some basic example applications on the STM32F4 series. Tested in 32F429IDISCOVERY.
# ------------------------------------------------------
* TP1 - INPUT/OUTPUT      ----------------------
  -  When we push the Blue Button of the STM29F429I_Discovery, interfaced with the pin PA0 the green LED (pin LD3) has to be ON, and the red one (LD4) has to be toggeled.
  -  WHen we release the Blue Button (PA0) we turn Off the 2 LEDs.

Use: STM32CubeMX and ARM Keil v5
# ------------------------------------------------------
* TP2 - UART Communication    ------------------
  - Exchange a char table, whose size is defined by SIZE (5) bitween UART5 and USART1.
  the UART pins are interfaced in the STM29F429I_Discovery like that:
  
  - PC12 ------> UART5_TX    AND    PD2 ------> UART5_RX
  - PA9 ------> USART1_TX    AND    PA10 ------> USART1_RX

Use: STM32CubeMX and ARM Keil v5
# ------------------------------------------------------
