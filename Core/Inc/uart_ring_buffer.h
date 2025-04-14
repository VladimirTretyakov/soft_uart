#ifndef UART_RING_BUFFER_H
#define UART_RING_BUFFER_H

#include <stdint.h>
#include <stdbool.h>

#define UART_RX_BUF_SIZE 64

void uart_buf_push(uint8_t byte);          // add byte to buff
bool uart_buf_pop(uint8_t *out);           // insert byte
bool uart_buf_has_data(void);              // is there any data?

bool uart_receive_byte(uint8_t *byte_ptr);

#endif
