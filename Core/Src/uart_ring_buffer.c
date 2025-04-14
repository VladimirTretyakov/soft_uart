#include "uart_ring_buffer.h"

static uint8_t buffer[UART_RX_BUF_SIZE];
static volatile uint8_t head = 0;
static volatile uint8_t tail = 0;

void uart_buf_push(uint8_t byte) {
    uint8_t next = (head + 1) % UART_RX_BUF_SIZE;
    if (next != tail) {
        buffer[head] = byte;
        head = next;
    }
    // if the buffer is full - ignore the byte (or you can add an error counter)
}

bool uart_buf_pop(uint8_t *out) {
    if (head == tail) return false;
    *out = buffer[tail];
    tail = (tail + 1) % UART_RX_BUF_SIZE;
    return true;
}

bool uart_buf_has_data(void) {
    return head != tail;
}

bool uart_receive(uint8_t *byte_ptr) {
    return uart_buf_pop(byte_ptr);
}

