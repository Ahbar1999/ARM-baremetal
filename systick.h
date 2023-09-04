#ifndef _SYSTICK_H_
#define _SYSTIC_H_
#include <stdint.h>

void configure_systick(uint32_t reload_value, uint8_t enable_int);

#endif
