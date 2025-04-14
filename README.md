# soft_uart
Software UART over GPIO for STM32G070RB
=======================================

This project implements a software-based UART using GPIO pins on the STM32 Nucleo G070RB board.
It allows UART functionality on platforms where hardware UART may be unavailable or limited.

Pin Configuration
-----------------

The default pin assignments are:

| Function | Pin |
|----------|-----|
| TX       | PA0 |
| RX       | PA1 |
| CTS      | PA2 |
| RTS      | PA3 |
| LED      | PA5 |

These can be changed in `main.h`:

#define TX_Pin        GPIO_PIN_0
#define TX_GPIO_Port  GPIOA
#define RX_Pin        GPIO_PIN_1
#define RX_GPIO_Port  GPIOA
#define RX_EXTI_IRQn  EXTI0_1_IRQn
#define CTS_Pin       GPIO_PIN_2
#define CTS_GPIO_Port GPIOA
#define RTS_Pin       GPIO_PIN_3
#define RTS_GPIO_Port GPIOA
#define LED_Pin       GPIO_PIN_5
#define LED_GPIO_Port GPIOA

Usage
-----

Include the required headers:

    #include "soft_uart.h"
    #include "uart_ring_buffer.h"

Timer Configuration
-------------------

Two timers are needed:
- TX Timer – for controlling signal timing during transmission.
- RX Timer – for sampling during reception.

Reception is handled via an external interrupt, which means the UART does not block the main loop.

> You can technically use only one timer, but for baud rates ≥ 57600, this may lead to data collisions.

Example Initialization:

    MX_TIM1_Init();
    MX_TIM3_Init();

    HAL_TIM_Base_Start(&htim1);
    HAL_TIM_Base_Start(&htim3);

Baud Rate Configuration
-----------------------

To change the baud rate, modify the `uart_baud_us` variable in `soft_uart.c`:

    volatile uint32_t uart_baud_us = b9600; // For 9600 baud


Receive Buffer
--------------

A 64-byte circular buffer is implemented for RX. The reception works as a FIFO.

Example usage in the main loop (simple echo):

    uint8_t b;
    if (uart_buf_pop(&b)) {
        uart_send(b); // Echo received byte
    }

Features
--------

- TX/RX over GPIO (bit-banging)
- CTS/RTS hardware flow control support
- External interrupt-based RX start
- Timer-driven sampling and transmission
- Circular buffer for non-blocking RX

Dependencies
------------

Make sure your project is set up with the following HAL modules enabled in `stm32g0xx_hal_conf.h`:
- HAL_TIM_MODULE_ENABLED
- HAL_GPIO_MODULE_ENABLED
- HAL_EXTI_MODULE_ENABLED
