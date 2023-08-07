#include <stdint.h>
#include "main.h"
#include "blink.h"

const uint32_t constant = 10;
uint32_t init_var = 1;
uint32_t uninit_var;

int main(void) {
	int a = 0;
	a += 1;
	
    // call blink
    reset();
	// LD3 led
    blink(9);
	
	while(1);

    return 0;
}
