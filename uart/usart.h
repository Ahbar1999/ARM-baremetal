#ifndef _USART_H_
#define _USART_H_

void configure_usart1(uint16_t baud_rate);
void configure_usart2(uint16_t baud_rate);
void send_message(uint8_t* msg, uint32_t size);
void echo();

#endif
