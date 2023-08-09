#include <stdint.h>
#include "main.h"
#include "blink.h"

// #include "uart/usart.h"

// NVIC configuration
#define ISER_ADDR  			0xE000E100
#define SCB_START_ADDR		0xE000ED00
#define SCB_SHPR3_OFFSET	0x20
#define SCB_SHPR2_OFFSET	0x1C
#define SCB_SHPR2_RV		0x00000000U
#define SCB_SHPR3_RV		0x00000000U

volatile uint32_t* ISER = (uint32_t*)ISER_ADDR;	// for enabling any non-system interrupts 
volatile uint32_t* SCB_SHPR2 = (uint32_t*)(SCB_START_ADDR + SCB_SHPR2_OFFSET);  
volatile uint32_t* SCB_SHPR3 = (uint32_t*)(SCB_START_ADDR + SCB_SHPR3_OFFSET);  

const uint32_t constant = 10;
uint32_t init_var = 1;
uint32_t uninit_var;
uint8_t timer_counter = 0;

// Pin PC9 -> LD3 led
const uint8_t LED = 9;

uint8_t message[] = {'c', 'o', 'u', 'n', 't', 'e', 'r', ':', ' ', 0, '\0'};

/*
// remember to declare this in header file
void SysTick_Handler(void) {
	message[10]	= timer_counter++;
	send_message(message, sizeof(message));	
}
*/

void SysTick_Handler(void) {
	// use BKPT instruction to add a breakpoint here 
	// toggle operation
	// call blink
    blink(LED);
}

void NMI_Handler(void) {
	while(1);
}

void HardFault_Handler(void) {
	// __asm volatile("BKPT");
	while(1);
}

void SVCall_Handler(void) {
	while(1);
}

void PendSV_Handler(void) {
	while(1);
}

int main(void) {
	// enable/configure any interrutps
	*SCB_SHPR2 = SCB_SHPR2_RV;
	*SCB_SHPR3 = SCB_SHPR3_RV;
	// __asm volatile("BKPT");
   	// SVCall priority = 3	
	// *SCB_SHPR2 |= 3 << 28;
	// SysTick priority = 6
	// *SCB_SHPR3 |= 6 << 28;
	// PendSV priority = 5
	// *SCB_SHPR3 |= 5 << 20;
	
	reset(LED);
	blink(LED);
	
	while(1);

    return 0;
}
