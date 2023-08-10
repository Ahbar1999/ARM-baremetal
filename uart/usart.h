#ifndef _USART_H_
#define _USART_H_

void configure_usart(uint16_t baud_rate);
void send_message(uint8_t* msg, uint32_t size);

#endif
