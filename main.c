#ifndef _MAIN_
#define _MAIN_

#include <stdint.h>
#include "blink.c"

extern int main(void) {
	int a = 0;
	a += 1;
	
    // call blink
    reset();
    blink(0);
	
	while(1);

    return 0;
}

#endif
