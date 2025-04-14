#include "soft_uart.h"
#include "uart_ring_buffer.h"

volatile uint8_t uart_receiving = 0;

volatile uint32_t uart_baud_us = b9600;
static volatile uint8_t rx_byte = 0;
static volatile uint8_t rx_bit_index = 0;
static volatile uint8_t rx_ready = 0;

extern TIM_HandleTypeDef htim1;

void uart_gpio_init(void) {
    HAL_GPIO_WritePin(TX_GPIO_Port, TX_Pin, GPIO_PIN_SET);

    // RTS on (ready recevie)
    HAL_GPIO_WritePin(RTS_GPIO_Port, RTS_Pin, GPIO_PIN_SET);
}

static inline void delay_us(uint32_t us) {
    uint32_t start = TIM3->CNT;
    while ((uint16_t)(TIM3->CNT - start) < us);
}

void uart_send(uint8_t byte) {
	if ((CTS_GPIO_Port->IDR & CTS_Pin) == 0)
	    return;

    TX_GPIO_Port->BRR = TX_Pin; // Start bit
    delay_us(uart_baud_us);

    for (uint8_t i = 0; i < 8; i++) {
    	if ((byte >> i) & 1)
    	    TX_GPIO_Port->BSRR = TX_Pin;  // set HIGH on Tx pin
    	else
    	    TX_GPIO_Port->BRR = TX_Pin;   // set LOW on Tx pin

        delay_us(uart_baud_us);
    }

    TX_GPIO_Port->BSRR = TX_Pin; // Stop bit
    delay_us(uart_baud_us);
}

void uart_rx_sample_bit(void) {
    if (rx_bit_index < 8) {
        uint8_t bit = (RX_GPIO_Port->IDR & RX_Pin) ? 1 : 0;
        rx_byte |= (bit << rx_bit_index);
        rx_bit_index++;
    } else {
        // verify stop bit
        uint8_t stop_bit = (RX_GPIO_Port->IDR & RX_Pin) ? 1 : 0;
        if (stop_bit) {
        	uart_buf_push(rx_byte); //send to ring buff, byte received
        } else {
            rx_ready = 0;     // error
            rx_byte = 0xFF;
        }

        uart_receiving = 0;   // all skip

        // stop timer
        TIM1->CR1 &= ~TIM_CR1_CEN;
        TIM1->DIER &= ~TIM_DIER_UIE;
        TIM1->SR &= ~TIM_SR_UIF;
    }
}


volatile void uart_start_receive(void) {
    rx_byte = 0;
    rx_bit_index = 0;
    rx_ready = 0;

    uart_receiving = 1; // in process receive

    //delay_us(uart_baud_us / 2);  // maybe use if you set 57600 and highest

    TIM1->CNT = 0;               // reset counter
    TIM1->ARR = uart_baud_us;   // set period like as bit
    TIM1->SR &= ~TIM_SR_UIF;    // clear interrupt flag (Update)
    TIM1->DIER |= TIM_DIER_UIE; // enable overflow interrupt

    TIM1->CR1 |= TIM_CR1_CEN;   // start timer
}


void uart_set_rts(uint8_t state) {
    if (state)
        RTS_GPIO_Port->BSRR = RTS_Pin;  // set RTS = 1
    else
        RTS_GPIO_Port->BRR  = RTS_Pin;  // set RTS = 0
}


uint8_t uart_data_ready(void) {
    return rx_ready;
}

uint8_t uart_get_rx_byte(void) {
    rx_ready = 0;
    return rx_byte;
}


