#include <stdint.h>
#include "main.h"
#include "gpio/gpio.h"
#include "uart/usart.h"

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

volatile uint32_t* ISER = (uint32_t*)ISER_ADDR;	// for enabling any non-system interrupts 
volatile uint32_t* SCB_SHPR2 = (uint32_t*)(SCB_START_ADDR + SCB_SHPR2_OFFSET);  
volatile uint32_t* SCB_SHPR3 = (uint32_t*)(SCB_START_ADDR + SCB_SHPR3_OFFSET);  

// uint32_t init_var = 1;
// uint32_t uninit_var;
// PIN DEFINITIONS
// Pin PC9 -> LD3 led
const uint8_t LED = 9;	// PC9
const uint8_t USART1_TX = 9;	//PA9 
const uint8_t USART1_RX = 10;	// PA10
const uint8_t USART2_TX = 2;	//PA2 
const uint8_t USART2_RX = 3;	// PA3


uint8_t message[] = {'c', 'o', 'u', 'n', 't', 'e', 'r', ':', ' ', 0, '\r'};
extern volatile uint8_t usart1_configured;
extern volatile uint8_t usart2_configured;

#define GPIOA 0
#define GPIOB 1 
#define GPIOC 2

void SysTick_Handler(void) {
	// use BKPT instruction to add a breakpoint here 
	// toggle operation
	// call blink
	// __asm volatile("BKPT");
	toggle_pin(LED, GPIOC);
	/*	
	message[9] += 1;
	if (usart_configured) {
		send_message(message, sizeof(message));
	}
	*/
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

void PendSV_Handler(void) {
	while(1);
}

int main(void) {
	// enable/configure any interrutps
	// set priorty registers to reset value	
	*SCB_SHPR2 = SCB_SHPR2_RV;
	*SCB_SHPR3 = SCB_SHPR3_RV;
	// __asm volatile("BKPT");
  	reset_gpio(); 

	// configure pins
	// you also need to tell which GPIO it is, A, B, C etc
	configure_pin(LED, 1, GPIOC);
	
	configure_usart1(BAUD_RATE1);
	configure_usart2(BAUD_RATE2);
	// 2: AF(alternate function) mode
	configure_pin(USART1_TX, 2, GPIOA);
	configure_pin(USART1_RX, 2, GPIOA);
	
	configure_pin(USART2_TX, 2, GPIOA);
	configure_pin(USART2_RX, 2, GPIOA);	
	// enable interrupt on uart1
	// uart_enable_interrupt(0);
	echo();
	while(1);
    
	return 0;
}
