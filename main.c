#include <stdint.h>
#include "main.h"
#include "blink.h"
#include "uart/usart.h"



// NVIC configuration
#define ISER_ADDR  			0xE000E100
#define SCB_START_ADDR		0xE000ED00
#define SCB_SHPR3_OFFSET	0x20
#define SCB_SHPR3_RV		0x0

volatile uint32_t* ISER = (uint32_t*)ISER_ADDR; 
volatile uint32_t* SCB_SHPR3 = (uint32_t*)(SCB_START_ADDR + SCB_SHPR3_OFFSET);  

const uint32_t constant = 10;
uint32_t init_var = 1;
uint32_t uninit_var;
uint8_t timer_counter = 0;

uint8_t message[] = {'c', 'o', 'u', 'n', 't', 'e', 'r', ':', ' ', 0, '\0'};

/*
void SysTick_Handler(void) {
	message[10]	= timer_counter++;
	send_message(message, sizeof(message));	
}
*/

int main(void) {
	// enable/configure any interrutps
	*SCB_SHPR3 = (uint32_t)SCB_SHPR3_RV;

    // call blink
    reset();
	// LD3 led
    blink(9);
	
	while(1);

    return 0;
}
