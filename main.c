#include <stdint.h>
#include "main.h"
#include "blink.h"

int main(void) {
	int a = 0;
	a += 1;
	
    // call blink
    reset();
    blink(0);
	
	while(1);

    return 0;
}
