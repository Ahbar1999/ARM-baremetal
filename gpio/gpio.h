#ifndef _GPIO_H_
#define _GPIO_H_

#include <stdint.h>

void configure_pin(uint8_t pin, uint8_t mode, uint8_t port);
void toggle_pin(uint8_t pin, uint8_t port);
void reset_gpio(void);

#endif
