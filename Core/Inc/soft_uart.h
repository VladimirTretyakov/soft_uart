#ifndef __SOFT_UART_H
#define __SOFT_UART_H

#include "main.h"

extern volatile uint32_t uart_baud_us;
extern volatile uint8_t uart_receiving;

#define b1200 833
#define b2400 417
#define b4800 208
#define b9600 104
#define b14400 69
#define b19200 52
#define b28800 35
#define b38400 26
#define b57600 17
#define b115200 8

void uart_gpio_init(void);           // init GPIO
void uart_timer_init(void);          // init timer
void uart_tx_byte(uint8_t byte);     // send byte
void uart_rx_sample_bit(void);       // pars one bit RX
void uart_start_receive(void);       // start receive 1 byte
void uart_set_rts(uint8_t state);    // set RTS
uint8_t uart_data_ready(void);       // flag - byte receive ready
uint8_t uart_get_rx_byte(void);      // get receive byte

#endif // __SOFT_UART_H
