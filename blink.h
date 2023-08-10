#ifndef _BLINK_H_
#define _BLINK_H_

#include <stdint.h>

void configure_gpio(uint8_t pin);
void blink(uint8_t pin);

#endif
