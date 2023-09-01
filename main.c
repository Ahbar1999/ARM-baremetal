#include <stdint.h>
#include "main.h"
#include "gpio/gpio.h"
#include "uart/usart.h"
#include "systick.h"
#include "kernel/kernel.h"

// NVIC configuration
#define ISER_ADDR  			0xE000E100
#define SCB_START_ADDR		0xE000ED00
#define SCB_SHPR3_OFFSET	0x20
#define SCB_SHPR2_OFFSET	0x1C
#define SCB_SHPR2_RV		0x00000000U
#define SCB_SHPR3_RV		0x00000000U

// fck = 8MHz, 9600 baud rate, USARTDIV = BAUD RATE = 0x0341
#define BAUD_RATE1			(8000000) / 9600	
#define BAUD_RATE2			(8000000) / 115200

#define GPIOA 0
#define GPIOB 1 
#define GPIOC 2
// number of times Systick is called per second
// and tick_counter is incremented
#define TICKS_PER_SEC 1000U
#define STK_RV (8000000U / TICKS_PER_SEC)

volatile uint32_t* ISER = (uint32_t*)ISER_ADDR;	// for enabling any non-system interrupts 
volatile uint32_t* SCB_SHPR2 = (uint32_t*)(SCB_START_ADDR + SCB_SHPR2_OFFSET);  
volatile uint32_t* SCB_SHPR3 = (uint32_t*)(SCB_START_ADDR + SCB_SHPR3_OFFSET);  

// uint32_t init_var = 1;
// uint32_t uninit_var;
// PIN DEFINITIONS
// Pin PC9 -> LD3 led
const uint8_t LED1 = 9;	// PC9
const uint8_t LED2 = 8;	// PC8
const uint8_t USART1_TX = 9;	//PA9 
const uint8_t USART1_RX = 10;	// PA10
const uint8_t USART2_TX = 2;	//PA2 
const uint8_t USART2_RX = 3;	// PA3

// external variables
extern volatile uint32_t* STK_CVR;
extern volatile uint32_t* STK_CSR;
extern uint8_t systick_configured;

extern volatile uint8_t usart1_configured;
extern volatile uint8_t usart2_configured;

uint8_t message[] = {'c', 'o', 'u', 'n', 't', 'e', 'r', ':', ' ', 0, '\r'};

uint32_t tick_counter = 0;

void delay(uint32_t duration) {
	uint32_t curr = tick_counter;
	while ((tick_counter - curr) < duration);
}

uint32_t stack1[40];
void Blinky1() {
	while(1) {
		// __asm volatile("BKPT");
		delay(TICKS_PER_SEC / 4);
		toggle_pin(LED1, GPIOC);
		delay(TICKS_PER_SEC / 4);
	}
}

uint32_t stack2[40];
void Blinky2() {
	// __asm volatile("BKPT");
	while(1) {
		delay(TICKS_PER_SEC / 2);
		toggle_pin(LED2, GPIOC);
		delay(TICKS_PER_SEC / 2);
	}
}

void SysTick_Handler(void) {
	// use BKPT instruction to add a breakpoint here 
	// toggle operation
	tick_counter++;	
	__asm volatile("CPSID i");
	OS_sched();
	__asm volatile("CPSIE i");
}

void NMI_Handler(void) {
	while(1);
}

void HardFault_Handler(void) {
	while(1);
}

void SVCall_Handler(void) {
	while(1);
}

// defined in kernel.c for os purposes
/* 
void PendSV_Handler(void) {
	while(1);
}
*/

int main(void) {
	// enable/configure any interrutps
	// set priorty registers to reset value	
	*SCB_SHPR2 = SCB_SHPR2_RV;
	*SCB_SHPR3 = SCB_SHPR3_RV;
	// __asm volatile("BKPT");
  	// HIS or internal oscillator is used as clk source
	// for systick therefore clk freq if 8 MHz
	if (!systick_configured) {
		configure_systick(STK_RV);
	}

	reset_gpio(); 
	OS_init();
	// configure pins
	// you also need to tell which GPIO it is, A, B, C etc
	configure_pin(LED1, 1, GPIOC);	
	configure_pin(LED2, 1, GPIOC);

	// configure_usart1(BAUD_RATE1);
	// configure_usart2(BAUD_RATE2);
	// 2: AF(alternate function) mode
	// configure_pin(USART1_TX, 2, GPIOA);
	// configure_pin(USART1_RX, 2, GPIOA);
	
	// configure_pin(USART2_TX, 2, GPIOA);
	// configure_pin(USART2_RX, 2, GPIOA);	
	// enable interrupt on uart1
	// uart_enable_interrupt(0);
	// echo();
	
	OSThread blinky1_thread;
	OSThread blinky2_thread;
	OSThread_start(
			&blinky1_thread,
			Blinky1,
			stack1,
			sizeof(stack1));
	
	OSThread_start(
		&blinky2_thread,
		Blinky2,
		stack2,
		sizeof(stack2));
		
	while(1);
    
	return 0;
}
